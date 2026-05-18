#version 330 core

in float height;
in vec3 fragNormal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 fogColor;
uniform float fogStart;
uniform float fogEnd;
uniform vec3 lightDir;
uniform vec3 lightColor;

float layer(float a, float b, float h) {
    return smoothstep(a, b, h);
}

void main() {
    vec3 norm = normalize(fragNormal);
    float slope = 1.0 - norm.y;

    // =========================
    // CORES
    // =========================

    vec3 sand        = vec3(0.78, 0.74, 0.58);
    vec3 grassLow    = vec3(0.36, 0.55, 0.24);
    vec3 grassMid    = vec3(0.28, 0.46, 0.20);
    vec3 mountain    = vec3(0.34, 0.32, 0.30);
    vec3 snow        = vec3(0.93, 0.94, 0.96);
    vec3 color = sand;

    // =========================
    // PRAIA
    // =========================

    float beach = layer(-5.0, 8.0, height);
    color = mix(sand, grassLow, beach);

    // =========================
    // CAMPOS / FLORESTA
    // =========================

    float grassBlend = layer(15.0, 55.0, height);
    color = mix(color, grassMid, grassBlend);

    // =========================
    // ROCHA
    // =========================

    float rockHeight = layer(60.0, 95.0, height);
    float rockSlope = smoothstep(0.32, 0.58, slope);
    float rockMask = max(rockHeight, rockSlope);

    color = mix(color, mountain, rockMask);

    // =========================
    // NEVE
    // =========================

    float snowMask = layer(110.0, 145.0, height);
    snowMask *= (1.0 - rockSlope * 0.65);

    color = mix(color, snow, snowMask);

    // =========================
    // VARIAÇÃO NATURAL
    // =========================

    float altitudeTint = clamp(height / 150.0, 0.0, 1.0);

    color *= mix(0.92, 1.05, altitudeTint);

    // =========================
    // ILUMINAÇÃO
    // =========================

    vec3 light = normalize(-lightDir);

    float ambient = 0.30;
    float diffuse = max(dot(norm, light), 0.0);
    diffuse = pow(diffuse, 1.15);

    vec3 result = (ambient + diffuse) * color * lightColor;

    // =========================
    // FOG
    // =========================

    float dist = length(fragPos - cameraPos);
    float fogFactor = clamp((dist - fogStart) / (fogEnd - fogStart), 0.0, 1.0);
    fogFactor = fogFactor * fogFactor;

    vec3 final = mix(result, fogColor, fogFactor);
    FragColor = vec4(final, 1.0);
}