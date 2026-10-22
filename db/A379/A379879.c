/*
 * A379879  E.g.f. A(x) satisfies A(x) = exp(-x) + x*A(x)^2.
 * https://oeis.org/A379879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1: a(n) = (-1)^n + n * Σ_{k=0}^{n-1}
 *            binom(n-1, k) * a(k) * a(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static idx_t  seq_cap   = 0;
static idx_t  seq_last  = 0;

/* Return the nth term of OEIS A379879.  The sequence is memoised so that
   repeated calls are fast and non‑monotonic. */
seq_t a(idx_t n)
{
    /* initialise cache on first call and for index 0 */
    if (seq_cache == NULL) {
        seq_cap = (n + 1 > 128) ? (n + 1) : 128;
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        seq_cache[0] = (seq_t)1;
        seq_last = 0;
    } else if (n > seq_cap) {
        /* grow cache to next power of two > n */
        size_t new_cap = seq_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_buf = (seq_t *)realloc(seq_cache, sizeof(seq_t) * new_cap);
        seq_cache = new_buf;
        seq_cap = (idx_t)new_cap;
    }

    /* compute missing values up to n */
    for (idx_t k = seq_last + 1; k <= n; ++k) {
        if (k == 0) {
            seq_cache[0] = (seq_t)1;
            continue;
        }

        /* sum_{j=0}^{k-1} binom(k-1,j) * a(j) * a(k-1-j) */
        seq_t sum = 0;
        seq_t binom = (seq_t)1;                     /* C(k-1,0) */
        idx_t half = (k - 1) / 2;                   /* middle index */

        for (idx_t j = 0; j <= half; ++j) {
            seq_t prod = seq_cache[j] * seq_cache[k - 1 - j];
            seq_t term = prod * binom;
            if (j == k - 1 - j)          /* middle term appears once */
                sum += term;
            else                        /* mirror term, appear twice */
                sum += term * (seq_t)2;

            /* update binom for next j (unless last) */
            if (j != half) {
                binom = (binom * (seq_t)(k - 1 - j)) / (seq_t)(j + 1);
            }
        }

        /* a(k) = (-1)^k + k * sum */
        seq_t mult = (seq_t)k * sum;
        seq_t result = mult;

        if (k % 2 == 0)          /* even: add 1 */
            result += (seq_t)1;
        else {                   /* odd: subtract 1, guard against zero */
            if (result != 0)
                result -= (seq_t)1;
        }

        seq_cache[k] = result;
    }

    seq_last = n;
    return seq_cache[n];
}
