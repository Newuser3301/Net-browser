#version 330 core
// Minimal demo blur shader stub (not wired yet)
in vec2 vUV;
out vec4 FragColor;
uniform sampler2D uTex;

void main() {
  vec2 texel = 1.0 / vec2(textureSize(uTex, 0));
  vec3 c = texture(uTex, vUV).rgb * 0.36;
  c += texture(uTex, vUV + vec2(texel.x, 0)).rgb * 0.16;
  c += texture(uTex, vUV - vec2(texel.x, 0)).rgb * 0.16;
  c += texture(uTex, vUV + vec2(0, texel.y)).rgb * 0.16;
  c += texture(uTex, vUV - vec2(0, texel.y)).rgb * 0.16;
  FragColor = vec4(c, 1.0);
}