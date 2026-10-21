/*
 * A246895  Number of length 3+4 0..n arrays with some pair in every
 *          consecutive five terms totalling exactly n.
 * https://oeis.org/A246895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 2*a(n-1) +4*a(n-2) -10*a(n-3)
 *            -5*a(n-4) +20*a(n-5) -20*a(n-7) +5*a(n-8) +10*a(n-9) -4*a(n-10)
 *            -2*a(n-11) +a(n-12) with initial values a(1) … a(12) computed by
 *            a DP over 7‑length arrays (values 0..n, every 5‑term window
 *            contains a pair summing to n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 31, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   init_first12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../utils/init_first12.h"

/* Forward declaration of helper routine that computes the first 12 terms. */
static void init_first12(seq_t *memo);

/* Main function required by the judge. */
seq_t a(idx_t n)
{
    static seq_t *memo = NULL;      /* memoization array, index 0 unused. */
    static idx_t memo_cap = 0;      /* current capacity (size of memo). */

    /* Initialise the table with the first 12 terms at the very first call. */
    if (memo_cap == 0) {
        memo_cap = 13;                       /* indices 0 … 12  (0 unused) */
        memo = (seq_t*)calloc((size_t)memo_cap, sizeof(seq_t));
        init_first12(memo);
    }

    /* Simple lookup for the initial block. */
    if (n <= 12)
        return memo[(idx_t)n];

    /* Extend the memo table if a larger index is requested. */
    if (n >= memo_cap) {
        seq_t *new_mem = (seq_t*)realloc(memo, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_mem)     /* Allocation failure – return 0. */
            return (seq_t)0;
        memo = new_mem;
        for (idx_t k = memo_cap; k <= n; ++k) {
            seq_t a1  = memo[k - 1];
            seq_t a2  = memo[k - 2];
            seq_t a3  = memo[k - 3];
            seq_t a4  = memo[k - 4];
            seq_t a5  = memo[k - 5];
            seq_t a7  = memo[k - 7];
            seq_t a8  = memo[k - 8];
            seq_t a9  = memo[k - 9];
            seq_t a10 = memo[k -10];
            seq_t a11 = memo[k -11];
            seq_t a12 = memo[k -12];

            seq_t sumPos = a1 * 2 + a2 * 4 + a5 * 20 + a8 * 5 + a9 * 10 + a12 * 1;
            seq_t sumNeg = a3 * 10 + a4 * 5 + a7 * 20 + a10 * 4 + a11 * 2;
            memo[k] = sumPos - sumNeg;          /* unsigned wrap is safe; sumPos ≥ sumNeg. */
        }
        memo_cap = n + 1;
    }
    return memo[n];
}
