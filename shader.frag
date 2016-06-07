#version 330 core

struct Material {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
	vec3 direction;
	vec3 position;
	float cutOff;
	float outerCutOff;

	float exponent;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 4

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 plantPos;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform int lightMode;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // == ======================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == ======================================
    // Phase 1: Directional lighting
    vec3 result;
	if( lightMode == -1){
	//result = vec3(FragPos.x / 255.0f,FragPos.y / 255.0f, FragPos.z / 255.0f);
		//float length = plantPos.x * plantPos.x + plantPos.y * plantPos.y + plantPos.z * plantPos.z;
		//result =  vec3(((118.0f / 255.0f) * length), 169.0f / 255.0f, 18.0f / 255.0f);;
		if(FragPos.x > .1f || FragPos.x < -.1f || FragPos.z > .1f || FragPos.z < -.1f || FragPos.y > 5.0f){
			result = vec3(118.0f / 255.0f, 169.0f / 255.0f, 18.0f / 255.0f);
		} else{
			result = vec3(122.0f / 255.0f, 72.0f / 255.0f, 15.0f / 255.0f);
		}
		//float length = fragPos.x * fragPos.x + fragPos.y * fragPos.y + fragPos.z * fragPos.z;
		
	}
	 else if( lightMode == 1 )
		result = CalcDirLight(dirLight, norm, viewDir);

	else if( lightMode == 2)
		// Phase 2: Point lights
		for(int i = 0; i < NR_POINT_LIGHTS; i++)
			result += CalcPointLight(pointLights[0], norm, FragPos, viewDir);    
    
	// Phase 3: Spot light
	else if( lightMode == 3)
		result = CalcSpotLight(spotLight, norm, FragPos, viewDir); 

	else
		result = CalcDirLight(dirLight, norm, viewDir);
    
    color = vec4(result, 1.0);	
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
	// Combine results
	// L(a) * k(a)
    vec3 ambient = light.ambient * material.ambient;
    
	// l(d) * diff * k(d)
	vec3 diffuse = light.diffuse * diff * material.diffuse;

	// l(s) * spec * k(s)
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.quadratic * (distance * distance));
    // Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

//
// Code adapted from
// https://github.com/JoeyDeVries/LearnOpenGL/blob/d5c3be70ab2b884cf2b2c94cbf73a31f632fbf47/src/2.lighting/6.multiple_lights/multiple_lights.frag
//
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
	// Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
	// Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.quadratic * (distance * distance));    
    
	// Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    
	// Check for going outside spotlight bounds
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
	// Combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return 3.0 * (ambient + diffuse + specular);
}