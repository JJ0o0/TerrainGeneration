#version 330 core

in vec3 fragPos;
in vec3 fragNormal;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 fogColor;
uniform float fogStart;
uniform float fogEnd;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform float time;

void main() {
    vec3 waterColor = vec3(0.15, 0.45, 0.72);

    vec3 norm = normalize(vec3(
        sin(fragPos.x * 0.1 + time) * 0.1,
        1.0,
        cos(fragPos.z * 0.1 + time) * 0.1
    ));
    vec3 light = normalize(-lightDir);
    float ambient = 0.4;
    float diffuse = max(dot(norm, light), 0.0);
    vec3 result = (ambient + diffuse * 0.5) * waterColor * lightColor;

    float dist = length(fragPos - cameraPos);
    float fogFactor = clamp((dist - fogStart) / (fogEnd - fogStart), 0.0, 1.0);
    fogFactor = fogFactor * fogFactor;
    result = mix(result, fogColor, fogFactor);

    FragColor = vec4(result, 0.75);
}