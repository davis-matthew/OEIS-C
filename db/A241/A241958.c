/*
 * A241958  Duplicate of A217800.
 * https://oeis.org/A241958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n+2)(n+3)/(2(2n+1)(2n+3)) * A005789(n+1), A005789(m) =
 *            2(3m)!/(m!(m+1)!(m+2)!); i.e. a(n) =
 *            (n+2)(n+3)(3n+3)!/((n+1)!(n+2)!(n+3)!(2n+1)(2n+3)), evaluated
 *            via prime exponents
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 30,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 164 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static long long A241958_leg(uint64_t m, uint64_t p) {
    long long e = 0;
    while (m > 0) {
        m /= p;
        e += (long long)m;
    }
    return e;
}

static long long A241958_val(uint64_t c, uint64_t p) {
    long long e = 0;
    while (c > 0 && c % p == 0) {
        c /= p;
        e++;
    }
    return e;
}

seq_t a(idx_t n) {
    uint64_t N = 3 * (uint64_t)n + 3;
    unsigned char *comp = (unsigned char *)calloc((size_t)N + 1, 1);
    seq_t r = (seq_t)1;
    uint64_t p, q;
    long long i;
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (q = p * p; q <= N; q += p) comp[q] = 1;
        long long e = A241958_leg(N, p) - A241958_leg(n + 1, p) - A241958_leg(n + 2, p)
                    - A241958_leg(n + 3, p)
                    + A241958_val(n + 2, p) + A241958_val(n + 3, p)
                    - A241958_val(2 * (uint64_t)n + 1, p) - A241958_val(2 * (uint64_t)n + 3, p);
        for (i = 0; i < e; i++) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
