/*
 * A322266  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = denominator of Sum_{j=1..n} 1/j^k.
 * https://oeis.org/A322266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = denominator of Σ_{j=1..m} 1/j^k where (m,k) are obtained
 *            by reading the square array A(n,k) by antidiagonals, starting
 *            with offset 1. The mapping is m = n - (d-1)*d/2, k = d - m,
 *            where d is the smallest integer with d*(d+1)/2 ≥ n
 * Range:     u8 n <= 23, u16 n <= 32, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_D 150  /* enough for n up to 10000 (d_max = 141) */

typedef struct { seq_t num; seq_t den; } Frac;

/* cache of computed fractions (m >= 1, k >= 0) */
static Frac cache[MAX_D + 1][MAX_D + 1];
static bool have[MAX_D + 1][MAX_D + 1] = {0};

/* recursive helper to compute fraction for (m,k) */
static void ensure_frac(idx_t m, idx_t k)
{
    if (have[m][k])
        return;

    if (m == 1) {
        /* Sum over j=1 only: always 1 */
        cache[m][k].num = 1;
        cache[m][k].den = 1;
        have[m][k] = true;
        return;
    }

    /* compute previous sum for (m-1, k) */
    ensure_frac(m - 1, k);
    Frac prev = cache[m - 1][k];

    /* denominator of the new term 1/m^k */
    seq_t term_den = 1;
    if (k > 0) {
        seq_t base = (seq_t)m;
        for (idx_t e = 0; e < k; ++e)
            term_den *= base;
    }

    /* lcm of prev.den and term_den */
    seq_t g = gcd(prev.den, term_den);
    seq_t lcm = (prev.den / g) * term_den;  /* lcm = prev.den * term_den / g */

    /* compute new numerator */
    seq_t new_num = prev.num * (lcm / prev.den) + (lcm / term_den); /* second term numerator is 1 */

    /* reduce */
    seq_t g2 = gcd(new_num, lcm);
    cache[m][k].num = new_num / g2;
    cache[m][k].den = lcm / g2;
    have[m][k] = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; undefined for n=0, return 0 */
        return 0;

    /* find d such that d*(d+1)/2 >= n */
    idx_t d = 1;
    while ((d * (d + 1) / 2) < n)
        ++d;

    idx_t m = n - ((d - 1) * d / 2);   /* 1‑based row index */
    idx_t k = d - m;                   /* 0‑based power index */

    /* safety check for array bounds */
    if (m > MAX_D || k > MAX_D)
        return 0; /* out of allocated bounds */

    ensure_frac(m, k);
    return cache[m][k].den;
}
