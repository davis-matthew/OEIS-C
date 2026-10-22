/*
 * A381620  a(n) is the number of solutions to the problem described in
 *          A381619 with smallest price equal to n.
 * https://oeis.org/A381620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) counts the number of nondecreasing triples (x,y,z) of
 *            positive integers with smallest term n that satisfy 10000*(x + y
 *            + z) = x*y*z
 * Range:     u8 n <= 150, u16 n <= 150, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Integer square‑root of a 64‑bit unsigned integer. */
static inline uint64_t isqrt_u64(uint64_t v)
{
    uint64_t r = (uint64_t)sqrt((long double)v);
    while ((r + 1) * (r + 1) <= v) ++r;
    while (r * r > v) --r;
    return r;
}

seq_t a(idx_t n_input)
{
    if (n_input == 0) return 0;          /* undefined, safety guard */
    const uint64_t K = 10000ULL;          /* 100^2 */

    uint64_t n = (uint64_t)n_input;

    /* y must satisfy n*y > K, i.e. y > K/n. */
    uint64_t start = n;
    uint64_t min_y = K / n + 1;
    if (min_y > start) start = min_y;

    /* Upper bound for y derived from z >= y : 
       y <= (K + sqrt(K*(K + n^2))) / n
    */
    uint64_t t = K * (K + n * n);
    uint64_t ss = isqrt_u64(t);
    uint64_t y_max = (K + ss) / n;
    if (start > y_max) return (seq_t)0;

    unsigned long long cnt = 0;

    for (uint64_t y = start; y <= y_max; ++y)
    {
        uint64_t num = K * (n + y);            /* numerator: K*(n + y) */
        uint64_t den = n * y - K;              /* denominator */
        if (den == 0) continue;                /* safety, should not happen */
        if (num % den != 0) continue;          /* z must be integral */
        uint64_t z = num / den;
        if (z < y) continue;                   /* enforce y <= z */
        ++cnt;                                  /* found a triple {n,y,z} */
    }

    return (seq_t)cnt;
}
