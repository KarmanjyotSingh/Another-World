#version 330 core
in vec2 TexCoords;
out vec4 color;
in vec3 playerPos;
uniform sampler2D sprite;
uniform vec3 spriteColor;
uniform int lightSwitch;
uniform vec3 lightSource;
void main()
{
    color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
    if(lightSwitch==0)
    {
        float dist = length(lightSource-playerPos);
        if(dist>100.0f){
            color = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }
}