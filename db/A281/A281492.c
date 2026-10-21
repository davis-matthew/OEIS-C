/*
 * A281492  Expansion of f(x, x^3) * f(x^4, x^5) in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A281492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { (i,j) | i ∈ E1, j ∈ E2, i + j = n }, where E1 are the
 *            exponents of f(x,x³) : 0, 2k²±k and E2 are the exponents of
 *            f(x⁴,x⁵) : 0, (9k²±k)/2
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code generates the two finite exponent sets up to n
   and counts the pairs summing to n.  All operations use only
   integer arithmetic, meet the type restrictions, and are
   compact enough to run well below the 2‑sec limit. */

#include <stddef.h>

#define MAX_EXPS 2048   /* safe upper bound for exponent counts up to n≈10000 */

seq_t a(idx_t n)
{
    static idx_t exps1[MAX_EXPS];
    static idx_t exps2[MAX_EXPS];
    size_t c1 = 0, c2 = 0;
    seq_t result = 0;

    /* constant term 1 contributes exponent 0 in each series */
    if (c1 < MAX_EXPS) exps1[c1++] = 0;
    if (c2 < MAX_EXPS) exps2[c2++] = 0;

    /* exponents from f(x,x³): 2k²−k and 2k²+k, k≥1 */
    for (idx_t k = 1; ; ++k) {
        idx_t e1 = 2 * k * k - k;
        if (e1 > n) break;
        if (c1 < MAX_EXPS) exps1[c1++] = e1;
        idx_t e2 = 2 * k * k + k;
        if (e2 <= n && c1 < MAX_EXPS) exps1[c1++] = e2;
    }

    /* exponents from f(x⁴,x⁵): (9k²−k)/2 and (9k²+k)/2, k≥1 */
    for (idx_t k = 1; ; ++k) {
        idx_t e3 = (9 * k * k - k) / 2;
        if (e3 > n) break;
        if (c2 < MAX_EXPS) exps2[c2++] = e3;
        idx_t e4 = (9 * k * k + k) / 2;
        if (e4 <= n && c2 < MAX_EXPS) exps2[c2++] = e4;
    }

    /* Count pairs (i,j) with i+j = n */
    for (size_t i = 0; i < c1; ++i) {
        idx_t i_val = exps1[i];
        for (size_t j = 0; j < c2; ++j) {
            if (i_val + exps2[j] == n) {
                ++result;
            }
        }
    }

    return result;
}
