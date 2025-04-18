#include <math.h>
#include <stdio.h>
#include <stdlib.h>  // card > aek.ppm

typedef int i;
typedef float f;

struct v {
    f x, y, z;
    v operator+(v r) { return v(x + r.x, y + r.y, z + r.z); }
    v operator*(f r) { return v(x * r, y * r, z * r); }
    f operator%(v r) { return x * r.x + y * r.y + z * r.z; }
    v() {}
    v operator^(v r) {
        return v(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
    }
    v(f a, f b, f c) {
        x = a;
        y = b;
        z = c;
    }
    v operator!() { return *this * (1 / sqrt(*this % *this)); }
};

i G[] = {218963, 283733, 292692, 283732, 218995, 0, 10064, 9584, 30544};

f R() { return (f)rand() / RAND_MAX; }

i T(v o, v d, f &t, v &n) {
    t = 1e9;
    i m = 0;
    f p = -o.z / d.z; 
    if (.01 < p) t = p, n = v(0, 0, 1), m = 1;
    for (i k = 19; k--;)
        for (i j = 9; j--;)
            if (G[j] & 1 << k) {
                v p = o + v(-k, 0, -j - 4);
                f b = p % d, c = p % p - 1, q = b * b - c;
                if (q > 0) {
                    f s = -b - sqrt(q);
                    if (s < t && s > .01) t = s, n = !(p + d * t), m = 2;
                }
            }
    return m;
}

v S(v o, v d) {
    f t;
    v n;
    i m = T(o, d, t, n);
    if (!m) return v(.7, .6, 1) * pow(1 - d.z, 4);
    v h = o + d * t, l = !(v(9 + R(), 9 + R(), 16) + h * -1),
      r = d + n * (n % d * -2);
    f b = l % n;
    if (b < 0 || T(h, l, t, n)) b = 0;
    f p = pow(l % r * (b > 0), 99);
    if (m & 1) {
        h = h * .2;
        return ((i)(ceil(h.x) + ceil(h.y)) & 1 ? v(3, 1, 1) : v(3, 3, 3)) *
               (b * .2 + .1);
    }
    return v(p, p, p) + S(h, r) * .5;
}

i main() {
    printf("P6 512 512 255 ");
    v g = !v(-6, -16, 0), a = !(v(0, 0, 1) ^ g) * .002, b = !(g ^ a) * .002,
      c = (a + b) * -256 + g;
    for (i y = 512; y--;)
        for (i x = 512; x--;) {
            v p(13, 13, 13);
            for (i r = 64; r--;) {
                v t = a * (R() - .5) * 99 + b * (R() - .5) * 99;
                p = S(v(17, 16, 8) + t,
                      !(t * -1 + (a * (R() + x) + b * (y + R()) + c) * 16)) *
                        3.5 +
                    p;
            }
            printf("%c%c%c", (i)p.x, (i)p.y, (i)p.z);
        }
}
