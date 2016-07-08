#version 330
#define M_PI 3.1415926535897932384626433832795

varying vec2 texCoord0;
varying vec4 pos0;
varying vec3 normal0;

uniform vec4 ambients[64];//Positions & Intensity
uniform vec4 ambientc[64];//Colors & Range

uniform vec4 splights[64];//Positions & Intensity
uniform vec4 splightc[64];//Colors & Range
uniform vec4 splightdir[64];//Direction & Radius

uniform sampler2D diffuse;
uniform int effect;

vec3 GetClosestPoint(vec3 line, vec3 point){
	float t = dot(point, line) / pow(length(line), 2);
	if (t < 0){ return vec3(0,0,0); }
	return line * t;
}

float GetAngle(vec2 origin, vec2 targ){//Enhanced arc tangent
	targ -= origin;
	if (targ.x == 0){
		if (targ.y > 0){
			return M_PI / 2;
		}else{
			return 3 * M_PI / 2;
		}
	}
	float temp = atan(abs(targ.y / targ.x));
	if (targ.x < 0 && targ.y > 0){
		temp = M_PI - temp;
	}else if (targ.x < 0 && targ.y < 0){
		temp += M_PI;
	}else if (targ.x > 0 && targ.y < 0){
		temp = 2 * M_PI - temp;
	}
	return temp;
}

void main(){
	vec4 color = texture2D(diffuse, texCoord0);
	
	switch(effect){//Beginning effects
		default:{
			break;
		}
		case 1:{
			gl_FragColor = color;
			return;
		}
	}
	
	vec4 tempColor;
	bool changed = false;
	float intensity = 0.01;
	for(int i = 0;i<64;i++){
		if(ambients[i].w == 0){ break; }
		changed = true;
		float result = distance(pos0.xyz, ambients[i].xyz);
		if(result <= ambientc[i].w){
			float ratio = pow(1 - result / ambientc[i].w, 2);
			intensity += ratio * ambients[i].w;
			tempColor += ambientc[i] * ratio * ambients[i].w;
			tempColor.w++;
		}
	}
	for(int i = 0;i<64;i++){
		if(splights[i].w == 0){ break; }
		changed = true;
		float result = distance(pos0.xyz, splights[i].xyz);
		if(result <= splightc[i].w){
			float ratio = 1 - (result / splightc[i].w);
			ratio *= splights[i].w;
			vec3 tempCenter, relPos;
			relPos = pos0.xyz - splights[i].xyz;
			
			float ang = abs(GetAngle(vec2(0,0), vec2(splightdir[i].x, splightdir[i].z)) - GetAngle(vec2(0,0), vec2(relPos.x, relPos.z)));
			if(ang > M_PI/2 && ang < M_PI*3/2){ continue; }
			
			tempCenter = GetClosestPoint(splightdir[i].xyz, relPos);
			
			result = distance(relPos, tempCenter);
			float radius = length(tempCenter) * splightdir[i].w;
			if(result <= radius){
				ratio *= (1 - pow(result, 2) / pow(radius, 2));
				intensity += ratio;
				tempColor += splightc[i] * ratio;
				tempColor.w++;
			}
		}
	}
	if(changed){
		if(intensity < 0.01){ intensity = 0.01; }
		if(tempColor.w > 0){
			tempColor.xyz /= tempColor.w;
			float ratio = 1 / max(max(tempColor.r, tempColor.g), tempColor.b);
			intensity *= ratio;
			color.xyz *= intensity * tempColor.xyz;
		}else{
			color.xyz *= intensity;
		}
	}
	switch(effect){//After effects
		default:{
			break;
		}
		case 2:{
			color.xyz = vec3(1,1,1) - color.xyz;
			break;
		}
		case 3:{
			float choice = (color.r + color.g + color.b)/3.0;
			color.xyz = vec3(choice, choice, choice);
			break;
		}
		case 4:{
			break;
		}
	}
	gl_FragColor = color;
}