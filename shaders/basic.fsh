#version 330 core
in vec2 v_uv;
out vec4 color;

uniform sampler2D screen;

void main() {
    float p = texture(screen, v_uv).r;
    color = vec4(p, p, p, 1.0);
}
