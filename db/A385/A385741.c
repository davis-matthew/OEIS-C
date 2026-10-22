/*
 * A385741  a(n) = Sum_{k=0..n} (binomial(n, k) mod 9).
 * https://oeis.org/A385741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} ( binomial(n,k) mod 9 )
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a vector with n+1 entries to hold the current
       row of Pascal's triangle modulo 9. */
    unsigned char *row = (unsigned char *)malloc((size_t)(n + 1) * sizeof(unsigned char));
    if (row == NULL)
        return 0;                 /* allocation failure – return zero (unlikely for n<=10000) */

    /* Row 0: [1] */
    row[0] = 1 % 9;

    /* Build rows 1..n in-place. Each update uses the previous row
       values to compute the new row modulo 9. */
    for (idx_t i = 1; i <= n; ++i) {
        /* Rightmost entry is always 1. */
        row[i] = 1 % 9;

        /* Update interior entries from right to left so that we never
           overwrite a value that is still needed. */
        for (idx_t k = i; k > 1; --k) {
            row[k - 1] = (row[k - 2] + row[k - 1]) % 9;
        }
        /* row[0] stays 1. */
    }

    /* Sum the entries of the final row. Each entry is in {0,…,8}. */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += (seq_t)row[k];
    }

    free(row);
    return sum;
}
