/*
 * A385618  G.f. A(x) satisfies A(x) = 1/( 1 - x*(A(2*x) + A(3*x)) ).
 * https://oeis.org/A385618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} (2^k+3^k) * a(k) * a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of a[0..n] and the powers of 2 and 3. */
static seq_t *memo = NULL;      /* a[0..n]              */
static idx_t  memo_len = 0;     /* number of cached elements */

static seq_t *pow2 = NULL;      /* 2^k for 0 <= k */
static seq_t *pow3 = NULL;      /* 3^k for 0 <= k */
static idx_t  pow_len = 0;      /* number of cached powers */

/* Compute a(n) with memoization and simple O(n^2) convolution. */
seq_t a(idx_t n)
{
    /* 1. Allocate / grow memo array to contain a[0..n]. */
    if (n + 1 > memo_len) {
        idx_t new_len = n + 1;

        seq_t *tmp = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        memo = tmp;

        /* 2. Allocate / grow power arrays to contain 2^k and 3^k for 0 <= k < new_len. */
        if (pow_len < new_len) {
            seq_t *tmp2 = (seq_t *)realloc(pow2, new_len * sizeof(seq_t));
            seq_t *tmp3 = (seq_t *)realloc(pow3, new_len * sizeof(seq_t));
            if (!tmp2 || !tmp3) return (seq_t)0;  /* allocation failure */
            pow2 = tmp2;
            pow3 = tmp3;

            /* Initialize base cases if this is the first time. */
            if (pow_len == 0) {
                pow2[0] = (seq_t)1;
                pow3[0] = (seq_t)1;
                pow_len = 1;
            }

            /* Compute missing powers. */
            for (idx_t i = pow_len; i < new_len; ++i) {
                pow2[i] = pow2[i - 1] * ((seq_t)2);
                pow3[i] = pow3[i - 1] * ((seq_t)3);
            }
            pow_len = new_len;
        }

        /* 3. Compute any new a[i] values. */
        for (idx_t i = memo_len; i < new_len; ++i) {
            if (i == 0) {
                memo[0] = (seq_t)1;
            } else {
                seq_t sum = (seq_t)0;
                for (idx_t k = 0; k < i; ++k) {
                    seq_t w = pow2[k] + pow3[k];
                    seq_t prod = memo[k] * memo[i - 1 - k];
                    prod *= w;
                    sum += prod;
                }
                memo[i] = sum;
            }
        }
        memo_len = new_len;
    }
    return memo[n];
}
