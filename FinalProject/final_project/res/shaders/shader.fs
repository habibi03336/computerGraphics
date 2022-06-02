#version 330 core
out vec4 FragColor;

// * frag shader의 position관련 값들은 world coordinate 기준이다.
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct SpotLight {
    vec3 position;
    vec3 direction;
};

struct SpotLightCommon {
    float cutOff;
    float innerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float attenConstant;
    float attenLinearTerm;
    float attenQuadTerm;
};

struct SunLight {
    vec3 color;
    vec3 pos;
};


struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

uniform SpotLight spotLight, spotLight2, spotLight3;
uniform SpotLightCommon spotLightCommon;
uniform int spotLightOn;
uniform SunLight sunlight;
uniform Material material;


uniform vec3 objectColor;
uniform vec3 viewPos;

vec4 calculateSpotLight(SpotLight light);
vec4 calculateSunLight();

vec4 fragSpotLight;
vec4 fragSunLight;


void main()
{
    vec4 fragColor_1 = calculateSpotLight(spotLight);
    vec4 fragColor_2 = calculateSpotLight(spotLight2);
    vec4 fragColor_3 = calculateSpotLight(spotLight3);
    
    fragSpotLight = (fragColor_1 + fragColor_2 + fragColor_3) / 3 ;
    fragSunLight = calculateSunLight();
    
    FragColor = fragSpotLight * spotLightOn + fragSunLight;
}

vec4 calculateSunLight()
{
    vec3 ambient = texture( material.diffuse, TexCoords ).rgb * sunlight.color;
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(sunlight.pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * sunlight.color;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * sunlight.color;
    
    // ambient + diffuse + specular
    vec3 result_ = (ambient + diffuse + specular);
    vec4 result = vec4(result_, 1.0);
    
    return result;
}

vec4 calculateSpotLight(SpotLight light)
{
    // 이 지점에 빛이 들어오는 방향
    vec3 lightDir = normalize(light.position - FragPos);
    float cos_theta = dot(lightDir, normalize(-light.direction));
    vec4 color;
    
    if (cos_theta > spotLightCommon.cutOff) {
        // ambient
        vec3 ambient = spotLightCommon.ambient * texture(material.diffuse, TexCoords).rgb;
        // diffuse
        vec3 norm = Normal;
        // 이 지점에 빛이 들어오는 방향 & 이 지점의 normal 사이의 각도
        // 사이 각이 90도 이상이면(dot 프로덕트가 0이면) 빛이 닿지 않는다. 따라서 max 적용
        float diff = max(dot(norm, lightDir), 0.0);
        //
        vec3 diffuse = spotLightCommon.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);
        
        // specular
        // 카메라의 위치와 현재 계산 대상이 되는 지점 사이의 벡터를 구한다.
        vec3 viewDir = normalize(viewPos - FragPos);
        // 빛의 방향과 지점의 normal에 따라 reflection의 방향 계산.
        vec3 reflectDir = reflect(-lightDir, norm);
        //
        float spec = 0;
        // 빛이 닿지 않으면 specular도 없음 따라서 diff > 0일 때만 spec 값을 계산.
        // 반사되는 방향, 보고있는 방향을 고려하여 specular의 크기를 계산. 사이 각이 줄어들 수록 spec도 커짐.
        if (diff > 0) spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        // 계산결과 및 설정 값, 빛 색깔을 고려하여 specular 계산
        vec3 specular = spotLightCommon.specular * (spec * material.specular);
        // ambient + diffuse + specular
        
        // smooth edge
        if (cos_theta < spotLightCommon.innerCutOff){
            float theta = acos(cos_theta);
            float cutOffAngle = acos(spotLightCommon.cutOff);
            float innerCutOffAngle = acos(spotLightCommon.innerCutOff);
            diffuse = diffuse * (theta - cutOffAngle) / (innerCutOffAngle - cutOffAngle);
            specular = specular * (theta - cutOffAngle) / (innerCutOffAngle - cutOffAngle);
        }
        
        // attenuation
        float distance = length(light.position - FragPos);
        float attenuation = 1.0 / (spotLightCommon.attenConstant + spotLightCommon.attenLinearTerm * distance + spotLightCommon.attenQuadTerm * (distance * distance));

        
        diffuse *= attenuation;
        specular *= attenuation;
        
        vec3 result = ambient + diffuse + specular;
        color = vec4(result, 1.0);
    } else {
        color = vec4(spotLightCommon.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
    }
    
    return color;
}

