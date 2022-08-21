#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // local to world coordinate
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;
    
    // Normal에 transition 변환을 제외하고 rotation 변환만 반영하는 식
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
