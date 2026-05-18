#version 330 core
in float height;
in vec3 fragNormal;
out vec4 FragColor;

void main() {
    vec3 color;

    if (height < -2.0) {
        // Água/areia
        color = vec3(0.76, 0.70, 0.50);
    } else if (height < 0.0) {
        // Grama baixa
        color = vec3(0.44, 0.62, 0.30);
    } else if (height < 4.0) {
        // Grama alta
        color = vec3(0.36, 0.54, 0.24);
    } else if (height < 7.0) {
        // Rocha
        color = vec3(0.50, 0.46, 0.42);
    } else {
        // Neve
        color = vec3(0.95, 0.95, 0.95);
    }

    FragColor = vec4(color, 1.0);
}