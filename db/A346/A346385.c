/*
 * A346385  a(n) = f(n,n) where f(0,n) = f(n,0) = n^n and f(m,n) = f(m-1,n) +
 *          f(m,n-1) + f(m-1,n-1).
 * https://oeis.org/A346385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n,n) where f(0,n) = f(n,0) = n^n f(m,n) = f(m-1,n) +
 *            f(m,n-1) + f(m-1,n-1) a(0) = 1, a(1) = 3, ... (OEIS A346385)
 *            This implementation uses a dynamic‑programming approach with two
 *            1‑dimensional arrays
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes the utilities described by the build are available */
#include <stdlib.h>
#include "../utils/big_pow.h"      /* seq_t big_pow(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == (idx_t)0) {
        return (seq_t)1;            /* 0^0 is defined as 1 for this sequence */
    }

    /* Allocate two rows of length n+1 each */
    seq_t *prev = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        /* In the unlikely event of allocation failure, return 0 */
        free(prev);
        free(curr);
        return (seq_t)0;
    }

    /* Initialise row 0: f(0,j) = j^j, with 0^0 = 1 */
    prev[0] = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j) {
        prev[j] = big_pow((seq_t)j, (seq_t)j);    /* j^j */
    }

    /* Iterate over rows 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        /* First column: f(i,0) = i^i */
        curr[0] = big_pow((seq_t)i, (seq_t)i);

        /* Remaining columns */
        for (idx_t j = 1; j <= n; ++j) {
            curr[j] = prev[j] + curr[j - 1] + prev[j - 1];
        }

        /* Swap pointers for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* The value in the last row at column n is a(n) */
    seq_t result = prev[n];

    /* Clean up */
    free(prev);
    free(curr);

    return result;
}
