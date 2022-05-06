#version 330 core
out vec4 FragColor;

// * frag shader의 position관련 값들은 world coordinate 기준이다.

in vec3 Normal;
in vec3 FragPos;
  
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform float specularPower;
uniform mat4 cameraRotation;
uniform vec3 viewPos;


void main()
{
    // ambient
    vec3 ambient = ambientStrength * lightColor;
      
    // diffuse
    vec3 norm = normalize(Normal);
    // 이 지점에 빛이 들어오는 방향
    vec3 lightDir = normalize(lightPos - FragPos);
    // 이 지점에 빛이 들어오는 방향 & 이 지점의 normal 사이의 각도
    // 사이 각이 90도 이상이면(dot 프로덕트가 0이면) 빛이 닿지 않는다. 따라서 max 적용
    float diff = max(dot(norm, lightDir), 0.0);
    // 사이 각도에 따라 lightColor가 얼마나 들어갈지 계산
    vec3 diffuse = diff * lightColor;
    
    // specular
    // 카메라의 위치와 현재 계산 대상이 되는 지점 사이의 벡터를 구한다.
    vec3 viewDir = normalize(viewPos - FragPos);
    // 빛의 방향과 지점의 normal에 따라 reflection의 방향 계산.
    vec3 reflectDir = reflect(-lightDir, norm);
    
    //
    float spec = 0;
    // 빛이 닿지 않으면 specular도 없음 따라서 diff > 0일 때만 spec 값을 계산.
    // 반사되는 방향, 보고있는 방향을 고려하여 specular의 크기를 계산. 사이 각이 줄어들 수록 spec도 커짐.
    if (diff > 0) spec = pow(max(dot(viewDir, reflectDir), 0.0), specularPower);
    
    // 계산결과 및 설정 값, 빛 색깔을 고려하여 specular 계산
    vec3 specular = specularStrength * spec * lightColor;
    
    // ambient + diffuse + specular
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}

