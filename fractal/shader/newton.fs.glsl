#version 330 core
out vec4 FragColor;

uniform float zz;           // Zoom
uniform vec2 center;        
uniform float width;
uniform float height;
uniform float sqrt3;

uniform float breathR;
uniform float breathG;
uniform float breathB;

uniform vec2 root1;
uniform vec2 root2;
uniform vec2 root3;

// (a + bi)(c + di) = (ac-bd) + (ad+bc)i
vec2 complexMul(vec2 a, vec2 b) {
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

// (a + bi)/(c + di) = ((ac+bd)/(c^2+d^2)) + ((bc-ad)/(c^2+d^2))i
vec2 complexDiv(vec2 a, vec2 b) {
    float denominator = b.x * b.x + b.y * b.y;
    return vec2((a.x * b.x + a.y * b.y) / denominator, (a.y * b.x - a.x * b.y) / denominator);
}

// Complex subtraction: (a + bi) - (c + di) = (a-c) + (b-d)i
vec2 complexSub(vec2 a, vec2 b) {
    return vec2(a.x - b.x, a.y - b.y);
}

bool checkRoot(vec2 z, vec2 root) {
    vec2 diff = z - root;
    // 1e-6 is r squared
    return (diff.x * diff.x + diff.y * diff.y) < 1e-6;
}

// Calculate f(z) = (z-r1)(z-r2)(z-r3)
vec2 f(vec2 z) {
    vec2 term1 = complexSub(z, root1);
    vec2 term2 = complexSub(z, root2);
    vec2 term3 = complexSub(z, root3);
    
    return complexMul(complexMul(term1, term2), term3);
}

// Calculate f'(z) = (z-r1)(z-r2) + (z-r1)(z-r3) + (z-r2)(z-r3)
vec2 fPrime(vec2 z) {
    vec2 term1 = complexMul(complexSub(z, root1), complexSub(z, root2));
    vec2 term2 = complexMul(complexSub(z, root1), complexSub(z, root3));
    vec2 term3 = complexMul(complexSub(z, root2), complexSub(z, root3));
    
    return term1 + term2 + term3;
}

void main() {
    // Pixel 2 normalizedDeviceCoords for Shader [-1;1]
    float aspect = width / height;
    vec2 uv = (gl_FragCoord.xy / vec2(width, height) - 0.5) * 2.0;
    uv.x *= aspect; // Aspect Ratio correction
    
    // zoom (zz) and pan (center)
    vec2 z = uv * zz + center;
    
    // const float SQRT3 = 1.732050807568877;
    vec2 r1 = vec2(-0.5, sqrt3 * 0.5);
    vec2 r2 = vec2(1.0, 0.0);
    vec2 r3 = vec2(-0.5, -sqrt3 * 0.5);
    
    // Newton iteration
    for(int i = 0; i < 100; i++) {
        // if(checkRoot(z, r1) || checkRoot(z, r2) || checkRoot(z, r3))
        if(checkRoot(z, root1) || checkRoot(z, root2) || checkRoot(z, root3))
            break;
            
        vec2 z2 = complexMul(z, z); // z^2
        vec2 z3 = complexMul(z2, z);// z^3
        // Newton: z - fx/fxdx
        z = z - complexDiv(f(z), fPrime(z));
    }
    
    // Color based on closest root
    vec3 color = vec3(0.0);
    // if(checkRoot(z, r1)) color.r = 1.0 * breathR;
    // if(checkRoot(z, r2)) color.g = 1.0 * breathG;
    // if(checkRoot(z, r3)) color.b = 1.0 * breathB;
    if(checkRoot(z, root1)) color.r = 1.0 * breathR;
    if(checkRoot(z, root2)) color.g = 1.0 * breathG;
    if(checkRoot(z, root3)) color.b = 1.0 * breathB;
    
    FragColor = vec4(color, 1.0);
}

