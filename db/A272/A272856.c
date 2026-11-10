/*
 * A272856  Greatest length of a chain of consecutive primes p==1 (mod 3) for
 *          which A261029 (2*prime(n)*p) is 4-i if prime(n) == i (mod 3),
 *          where i=1,2.
 * https://oeis.org/A272856
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = longest run of consecutive primes p == 1 (mod 3) with
 *            A261029(2*prime(n)*p) = 4 - (prime(n) mod 3). Computed by
 *            search. A261029(m) is the number of (u,v >= 0, not both 0) and
 *            divisors e of m with u^2+uv+v^2 = e, s = m/e >= 2u+v, and s ==
 *            2u+v (mod 3). For p > 4*prime(n)^2 the count no long...
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 52 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t c272856_isqrt(uint64_t x)
{
    uint64_t r = (uint64_t)__builtin_sqrt((double)x);
    while (r * r > x) --r;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

static int c272856_count(uint64_t m, uint64_t q, uint64_t p)
{
    uint64_t divs[32];
    int nd = 0;
    uint64_t f[3];
    int ex[3];
    int nf = 0;
    f[nf] = 2; ex[nf] = 1; nf++;
    if (p == q) {
        f[nf] = q; ex[nf] = 2; nf++;
    } else {
        f[nf] = q; ex[nf] = 1; nf++;
        f[nf] = p; ex[nf] = 1; nf++;
    }
    divs[nd++] = 1;
    for (int i = 0; i < nf; i++) {
        int cur = nd;
        uint64_t pw = 1;
        for (int k = 1; k <= ex[i]; k++) {
            pw *= f[i];
            for (int j = 0; j < cur; j++) divs[nd++] = divs[j] * pw;
        }
    }
    int cnt = 0;
    for (int d = 0; d < nd; d++) {
        uint64_t e = divs[d];
        uint64_t s = m / e;
        for (uint64_t u = 0; 2 * u <= s && u * u <= e; u++) {
            uint64_t D = 4 * e - 3 * u * u;
            uint64_t r = c272856_isqrt(D);
            if (r * r != D) continue;
            if (((r - u) & 1) != 0) continue;
            uint64_t v = (r - u) / 2;
            uint64_t t = 2 * u + v;
            if (s >= t && (s - t) % 3 == 0) cnt++;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* find prime(n) by trial division */
    uint64_t q = 1;
    uint64_t k = 0;
    while (k < (uint64_t)n) {
        q++;
        int isp = 1;
        for (uint64_t d = 2; d * d <= q; d++) {
            if (q % d == 0) { isp = 0; break; }
        }
        if (isp) k++;
    }
    int i = (int)(q % 3);
    int target = 4 - i;
    uint64_t L = 4 * q * q + 100;
    unsigned char *comp = (unsigned char *)malloc((size_t)L + 1);
    if (!comp) exit(EXIT_FAILURE);
    for (uint64_t j = 0; j <= L; j++) comp[j] = 0;
    for (uint64_t j = 2; j * j <= L; j++) {
        if (!comp[j]) {
            for (uint64_t l = j * j; l <= L; l += j) comp[l] = 1;
        }
    }
    uint64_t best = 0, run = 0;
    for (uint64_t p = 7; p <= L; p += 6) {
        if (comp[p]) continue;
        uint64_t m = 2 * q * p;
        if (c272856_count(m, q, p) == target) {
            run++;
            if (run > best) best = run;
        } else {
            run = 0;
        }
    }
    free(comp);
    return (seq_t)best;
}
