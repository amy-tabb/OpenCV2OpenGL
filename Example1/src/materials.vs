#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec4 ourColor;

uniform mat4 model_trans;
uniform mat4 extrinsic;
uniform mat4 intrinsic;

void main()
{
	ourColor = aColor;

  
    Normal = mat3(transpose(inverse(model_trans))) * aNormal;  
    
    FragPos = vec3(intrinsic*extrinsic*model_trans * vec4(aPos, 1.0));
       
   gl_Position = intrinsic * extrinsic * model_trans * vec4(aPos, 1.0);
 
}