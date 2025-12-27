#version 330 core
in vec2 v_uv;
out vec4 color;

uniform sampler2D screen;

void main() {
    vec2 uv = vec2(v_uv.x, 1.0 - v_uv.y);
    float p = texture(screen, uv).r;
    color = vec4(p, p, p, 1.0);
}
