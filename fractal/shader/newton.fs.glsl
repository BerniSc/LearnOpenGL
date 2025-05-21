#version 330 core
out vec4 FragColor;

uniform float zz;           // Zoom
uniform vec2 center;        
uniform float width;
uniform float height;
uniform float sqrt3;        // Might be some benefit in calulating this on cpu side first and then passing
                            // once as uniform

// Colour changning from CPU Side (breathing etc)
uniform float breathR;
uniform float breathG;
uniform float breathB;

uniform vec2 root1;
uniform vec2 root2;
uniform vec2 root3;

// (a + bj)(c + dj) = (ac-bd) + (ad+bc)j
vec2 complexMul(vec2 a, vec2 b) {
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

// (a + bj)/(c + dj) = ((ac+bd)/(c^2+d^2)) + ((bc-ad)/(c^2+d^2))j
vec2 complexDiv(vec2 a, vec2 b) {
    float denominator = b.x * b.x + b.y * b.y;
    return vec2((a.x * b.x + a.y * b.y) / denominator, (a.y * b.x - a.x * b.y) / denominator);
}

// Complex subtraction: (a + bj) - (c + dj) = (a-c) + (b-d)j
// Should be overloaded anyhow, but here we are^^ Safety first^^
vec2 complexSub(vec2 a, vec2 b) {
    return vec2(a.x - b.x, a.y - b.y);
}

// Complex addition: (a + bj) + (c + dj) = (a+c) + (b+d)j
// Should be overloaded anyhow, but here we are^^ Safety first^^
vec2 complexAdd(vec2 a, vec2 b) {
    return vec2(a.x + b.x, a.y + b.y);
}

bool checkRoot(vec2 z, vec2 root) {
    vec2 diff = z - root;
    // 1e-6 is r squared
    return (diff.x * diff.x + diff.y * diff.y) < 1e-6;
}

// f(z) = (z-r1)(z-r2)(z-r3)
vec2 f(vec2 z) {
    vec2 term1 = complexSub(z, root1);
    vec2 term2 = complexSub(z, root2);
    vec2 term3 = complexSub(z, root3);
    
    return complexMul(complexMul(term1, term2), term3);
}

// f'(z) = (z-r1)(z-r2) + (z-r1)(z-r3) + (z-r2)(z-r3)
vec2 fPrime(vec2 z) {
    vec2 term1 = complexMul(complexSub(z, root1), complexSub(z, root2));
    vec2 term2 = complexMul(complexSub(z, root1), complexSub(z, root3));
    vec2 term3 = complexMul(complexSub(z, root2), complexSub(z, root3));
    
    return complexAdd(complexAdd(term1, term2), term3);
}

vec2 fPrimePrime(vec2 z) {
    vec2 term1 = complexSub(z, root1);
    vec2 term2 = complexSub(z, root2);
    vec2 term3 = complexSub(z, root3);
    return 2 * (complexAdd(complexAdd(term1, term2), term3));
}

void main() {
    // Pixel2normalizedDeviceCoords for Shader [-1;1]
    float aspect = width / height;
    vec2 uv = (gl_FragCoord.xy / vec2(width, height) - 0.5) * 2.0;
    uv.x *= aspect; // Aspect Ratio correction
    
    // zoom (zz) and pan (center)
    vec2 z = uv * zz + center;
    
    // Can pin to achieve cool effects -> Bubbles etc. Need to check root againsts these tho and only check for these in color as well
    // Then cool optic
    // vec2 r1 = vec2(-0.5, sqrt3 * 0.5);
    // vec2 r2 = vec2(1.0, 0.0);
    // vec2 r3 = vec2(-0.5, -sqrt3 * 0.5);
    
    // Newton iteration
    for(int i = 0; i < 100; i++) {
        // Exit early if we already converge
        // if(checkRoot(z, r1) || checkRoot(z, r2) || checkRoot(z, r3))
        if(checkRoot(z, root1) || checkRoot(z, root2) || checkRoot(z, root3))
            break;
            
        // Newton: z - fx/fxdx
        // z = z - complexDiv(f(z), fPrime(z));

        // Different: z - (fxfxdxa)/(fxdx^2-fxfxdxdx)
        z = z - complexDiv(complexMul(f(z), fPrime(z)), \
                (complexSub(complexMul(fPrime(z), fPrime(z)), complexMul(f(z), fPrimePrime(z)))));
    }
    
    // Color based on closest root
    vec3 color = vec3(0.0);
    // if(checkRoot(z, r1)) color.r = 1.0 * breathR;
    // if(checkRoot(z, r2)) color.g = 1.0 * breathG;
    // if(checkRoot(z, r3)) color.b = 1.0 * breathB;
    if(checkRoot(z, root1)) color.r = 1.0 * breathR;
    if(checkRoot(z, root2)) color.g = 1.0 * breathG;
    if(checkRoot(z, root3)) color.b = 1.0 * breathB;

    // Visualize real roots
    // float epsilon = 1e-6;
    float epsilon = 1e-5;
    if(length(z - root1) < epsilon || length(z - root2) < epsilon || length(z - root3) < epsilon) {
        color = vec3(1.0, 1.0, 0.0); // Bright yellow for exact root
    }
    
    FragColor = vec4(color, 1.0);
}
