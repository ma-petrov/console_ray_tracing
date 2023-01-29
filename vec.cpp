struct vec2 {
    float x, y;

    vec2(float value) : x(value), y(value) {}
    vec2(float x_, float y_) : x(x_), y(y_) {}

    vec2 operator+ (vec2 const & other) {return vec2(x + other.x, y + other.y);}
    vec2 operator- (vec2 const & other) {return vec2(x - other.x, y - other.y);}
    vec2 operator* (vec2 const & other) {return vec2(x * other.x, y * other.y);}
    vec2 operator/ (vec2 const & other) {return vec2(x / other.x, y / other.y);}
    vec2 operator- () {return vec2(-x, -y);}
};

struct vec3 {
    float x, y, z;

    vec3(float value) : x(value), y(value), z(value) {}
    vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    vec3(float x_, vec2 uv) : x(x_), y(uv.x), z(uv.y) {}
    vec3(vec2 uv, float z_) : x(uv.x), y(uv.y), z(z_) {}

    vec3 operator+ (vec3 const & other) {return vec3(x + other.x, y + other.y, y + other.y);}
    vec3 operator- (vec3 const & other) {return vec3(x - other.x, y - other.y, y - other.y);}
    vec3 operator* (vec3 const & other) {return vec3(x * other.x, y * other.y, y * other.y);}
    vec3 operator/ (vec3 const & other) {return vec3(x / other.x, y / other.y, y / other.y);}
    vec3 operator- () {return vec3(-x, -y, -z);}
};

vec2 calculate_uv(int j, int i) {
    vec2 uv = vec2(j, i) * 2.0f;
    uv = uv / vec2(WIDTH - 1.0f, HEIGHT - 1.0f);
    uv = uv - 1.0f;
    uv = uv * vec2(ASPECT * PIXEL_ASPECT, 1);
    return uv;
}

float clamp(float value, float min, float max) { return fmax(fmin(value, max), min); }
double sign(double a) { return (0 < a) - (a < 0); }
double step(double edge, double x) { return x > edge; }

float length(vec2 const& v) { return sqrt(v.x * v.x + v.y * v.y); }
float length(vec3 const& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

float dot_product(vec3 const& a, vec3 const& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
vec3 norm(vec3 v) { return v / length(v); }
vec3 abs(vec3 const& v) { return vec3(fabs(v.x), fabs(v.y), fabs(v.z)); }
vec3 sign(vec3 const& v) { return vec3(sign(v.x), sign(v.y), sign(v.z)); }
vec3 step(vec3 const& edge, vec3 v) { return vec3(step(edge.x, v.x), step(edge.y, v.y), step(edge.z, v.z)); }
vec3 reflect(vec3 rd, vec3 n) { return rd - n * (2 * dot_product(n, rd)); }

vec3 rotateX(vec3 a, double angle)
{
    vec3 b = a;
    b.z = a.z * cos(angle) - a.y * sin(angle);
    b.y = a.z * sin(angle) + a.y * cos(angle);
    return b;
}

vec3 rotateY(vec3 a, double angle)
{
    vec3 b = a;
    b.x = a.x * cos(angle) - a.z * sin(angle);
    b.z = a.x * sin(angle) + a.z * cos(angle);
    return b;
}

vec3 rotateZ(vec3 a, double angle)
{
    vec3 b = a;
    b.x = a.x * cos(angle) - a.y * sin(angle);
    b.y = a.x * sin(angle) + a.y * cos(angle);
    return b;
}

vec2 sphere(vec3 ro, vec3 rd, float r) {
    float b = dot_product(ro, rd);
    float c = dot_product(ro, ro) - r * r;
    float h = b * b - c;
    if (h < 0.0) return vec2(-1.0);
    h = sqrt(h);
    return vec2(-b - h, -b + h);
}

vec2 box(vec3 ro, vec3 rd, vec3 boxSize, vec3& outNormal) {
    vec3 m = vec3(1.0) / rd;
    vec3 n = m * ro;
    vec3 k = abs(m) * boxSize;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = fmax(fmax(t1.x, t1.y), t1.z);
    float tF = fmin(fmin(t2.x, t2.y), t2.z);
    if (tN > tF || tF < 0.0) return vec2(-1.0);
    vec3 yzx = vec3(t1.y, t1.z, t1.x);
    vec3 zxy = vec3(t1.z, t1.x, t1.y);
    outNormal = -sign(rd) * step(yzx, t1) * step(zxy, t1);
    return vec2(tN, tF);
}

float plane(vec3 ro, vec3 rd, vec3 p, float w) {
    return -(dot_product(ro, p) + w) / dot_product(rd, p);
}