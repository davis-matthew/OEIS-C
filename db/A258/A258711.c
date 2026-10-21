/*
 * A258711  Motzkin numbers A001006 read mod 7.
 * https://oeis.org/A258711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Motzkin numbers modulo 7
 * Recurrence: M_0 = 1
 *              M_n = M_{n-1} + Σ_{k=0}^{n-2} M_k * M_{n-2-k}   (mod 7)
 * This function computes a(n) = M_n mod 7 for any n ≥ 0, memoising
 * intermediate values so that repeated calls are fast. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;     /* memoisation table */
    static idx_t cur_max = 0;      /* highest index already computed */
    static size_t capacity = 0;    /* allocated size of memo array */

    /* initialise the table on first call */
    if (memo == NULL) {
        capacity = 1;
        memo = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!memo) return 0;          /* allocation failure – should not happen */
        memo[0] = (seq_t)1;           /* M_0 = 1 */
        cur_max = 0;
    }

    /* grow the table if necessary */
    if ((size_t)n + 1 > capacity) {
        capacity = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, capacity * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – should not happen */
        memo = tmp;
    }

    /* compute any missing values up to n */
    for (idx_t k = cur_max + 1; k <= n; ++k) {
        /* start with the previous Motzkin number */
        seq_t sum = memo[k - 1] % (seq_t)7;

        /* add the convolution part only if k ≥ 2 */
        if (k >= 2) {
            for (idx_t i = 0; i <= k - 2; ++i) {
                seq_t prod = ( (memo[i] % (seq_t)7) *
                               (memo[k - 2 - i] % (seq_t)7) ) % (seq_t)7;
                sum += prod;
            }
        }

        memo[k] = sum % (seq_t)7;      /* store the result modulo 7 */
    }

    cur_max = n;
    return memo[n] % (seq_t)7;
}
