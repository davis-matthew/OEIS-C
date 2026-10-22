/*
 * A390350  Lower (1/2)-midsequence of F(n) and F(n+4), where F = A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A390350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) - a(n-5), with a(0)=1,
 *            a(1)=3, a(2)=4, a(3)=7, a(4)=12
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq.h"

seq_t a(idx_t n)
{
    /* Initial antecedents */
    static const seq_t init[5] = { (seq_t)1, (seq_t)3, (seq_t)4,
                                   (seq_t)7, (seq_t)12 };

    /* Handle the first five indices directly */
    if (n < 5) {
        return init[(size_t)n];
    }

    /* Static cache of previously computed terms */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;   /* capacity in elements  (>= n+1) */
    static size_t memo_filled = 0;/* number of values already stored */

    /* Grow the cache if necessary */
    size_t need = (size_t)n + 1;          /* indices 0 .. n inclusive */
    if (memo == NULL || need > memo_cap) {
        seq_t *p;
#ifdef __cplusplus
        p = static_cast<seq_t*>(realloc(memo, need * sizeof(seq_t)));
#else
        p = realloc(memo, need * sizeof(seq_t));
#endif
        if (!p) { /* Allocation failed – just return zero */
            return (seq_t)0;
        }
        memo = p;
        memo_cap = need;
    }

    /* Initialise the first five entries if not already done */
    if (memo_filled < 5) {
        for (size_t i = memo_filled; i < 5; ++i) {
            memo[i] = init[i];
        }
        memo_filled = 5;
    }

    /* Compute terms up to n */
    for (size_t i = memo_filled; i <= (size_t)n; ++i) {
        seq_t sum = memo[i-1] + memo[i-2] + memo[i-3];
        memo[i] = sum - memo[i-4] - memo[i-5];
    }
    memo_filled = need;    /* all required values are now cached */

    return memo[(size_t)n];
}
