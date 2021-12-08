#version 330 core
out vec4 color;

in vec4 f_color;
in vec2 f_texcoord;

uniform sampler2D u_texture;

void main() {
    color = texture(u_texture, f_texcoord) * f_color;
}