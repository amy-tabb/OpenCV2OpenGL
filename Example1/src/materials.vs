#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec4 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	ourColor = aColor;

  
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    FragPos = vec3(projection*view*model * vec4(aPos, 1.0));
       
   gl_Position = projection * view * model * vec4(aPos, 1.0);
 
}