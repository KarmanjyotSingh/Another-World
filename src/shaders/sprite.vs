#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec3 playerPos;
uniform mat4 model;
// note that we're omitting the view matrix; the view never changes so we basically have an identity view matrix and can therefore omit it.
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    playerPos=vec3(model*vec4(vertex.xy,0.0,1.0));
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);

}