#version 330 core
in vec2 v_uv;
out vec4 color;

uniform sampler2D screen;

void main() {
    vec2 uv = vec2(v_uv.x, 1.0 - v_uv.y);
    vec4 onColor = vec4(0.0, 1.0, 0.0, 1.0);
    vec4 offColor = vec4(0.05, 0.05, 0.05, 1.0);
    float p = texture(screen, uv).r;
    color = mix(offColor, onColor, p);
}
