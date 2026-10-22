/*
 * A345696  For 1<=x<=n, 1<=y<=n with gcd(x,y)=1, write 1 = gcd(x,y) = u*x+v*y
 *          with u,v minimal; a(n) = m^2*s, where s is the population variance
 *          of the values of u^2+v^2 and m is the number of such values.
 * https://oeis.org/A345696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m * Σt² – (Σt)², where m = # {(x,y) | 1 ≤ x,y ≤ n,
 *            gcd(x,y)=1}, t = u² + v² for the minimal solution u·x + v·y = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   mod_inverse_minimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_inverse_minimal.h"
/* Compute gcd of two idx_t numbers. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Use 128‑bit arithmetic for intermediate sums. */
    __int128 m = 0;          /* number of ordered coprime pairs */
    __int128 sum_t = 0;      /* Σ(u²+v²)  */
    __int128 sumsq_t = 0;    /* Σ(u²+v²)² */

    /* The pair (1,1). */
    m += 1;
    sum_t += 1;
    sumsq_t += 1;

    for (idx_t y = 2; y <= n; ++y) {
        for (idx_t x = 1; x < y; ++x) {
            if (gcd_idx(x, y) != 1) continue;

            int64_t u = mod_inverse_minimal(x, y);
            int64_t v = (1 - u * (int64_t)x) / (int64_t)y;   /* exact integer division */

            __int128 t = (__int128)u * u + (__int128)v * v;          /* u²+v² */

            m += 2;                            /* (x,y) and (y,x) */
            sum_t += 2 * t;
            sumsq_t += 2 * (__int128)t * t;
        }
    }

    __int128 result = m * sumsq_t - sum_t * sum_t;
    return (seq_t)result;
}
