#version 460 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec4 weights;
in ivec4 joints;

//  Contains the animated pose and inverse bind pose multiplied together (max 120 bones).
uniform mat4 animated[120];

out vec3 norm;
out vec3 fragPos;
out vec2 uv;

void main() 
{
    //  Build the skin matrix for all of the joints that influence this vertex.
    mat4 skin = animated[joints.x] * weights.x + animated[joints.y] * weights.y 
        + animated[joints.z] * weights.x * animated[joints.w] * weights.w;

    gl_Position = projection * view * model * skin * vec4(position, 1.0);
    
    fragPos = vec3(model * skin * vec4(position, 1.0));
    norm = vec3(model * skin * vec4(normal, 0.0f));
    uv = texCoord;
}


