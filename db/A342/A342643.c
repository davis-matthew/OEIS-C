/*
 * A342643  a(n) = [x^n] x * Product_{j>=0} (1 + x^(2^j) + n*x^(2^(j+1))).
 * https://oeis.org/A342643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = coefficient of x^n in x * Π_{j≥0} (1 + x^{2^j} +
 *            n·x^{2^{j+1}})
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 94, u64 n <= 510,
 *            u128 n <= 4094, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
# include <cstdlib>
#else
# include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition of the sequence */
    if (n == 0) return 0;

    /* target exponent for the product (exclude the outer x) */
    idx_t target = n - 1;

    /* array length */
    size_t len = (size_t)target + 1;

    /* dynamic arrays for DP: cur[s] holds coefficient for sum s */
    seq_t *cur = NULL;
    seq_t *next = NULL;
#ifdef __cplusplus
    cur = new seq_t[len]();   /* value‑initialized */
    next = new seq_t[len]();
#else
    cur = (seq_t *)calloc(len, sizeof(seq_t));
    next = (seq_t *)calloc(len, sizeof(seq_t));
#endif
    if (!cur || !next) return 0;   /* allocation failure – safe fallback */

    cur[0] = 1;                   /* coefficient of 0 for the empty product */

    seq_t sn = (seq_t)n;          /* copy n into seq_t for multiplications */

    /* iterate over powers of two up to target */
    for (idx_t pow2j = 1; pow2j <= target; pow2j <<= 1)
    {
        idx_t pow2j1 = pow2j << 1;   /* 2^(j+1) */

        /* zero the next array */
        for (idx_t s = 0; s <= target; ++s) next[s] = 0;

        /* transition from cur to next */
        for (idx_t s = 0; s <= target; ++s)
        {
            seq_t val = cur[s];
            if (val == 0) continue;

            /* choose 1 (exponent 0, coeff 1) */
            next[s] += val;

            /* choose x^{2^j} (coeff 1) */
            if (s + pow2j <= target) next[s + pow2j] += val;

            /* choose n·x^{2^{j+1}} (coeff n) */
            if (s + pow2j1 <= target) next[s + pow2j1] += val * sn;
        }

        /* swap arrays for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t result = cur[target];

    /* free resources */
#ifdef __cplusplus
    delete[] cur;
    delete[] next;
#else
    free(cur);
    free(next);
#endif

    return result;
}
