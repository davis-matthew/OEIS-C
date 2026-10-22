/*
 * A362495  Total number of blocks containing at least one odd element and at
 *          least one even element in all partitions of [n].
 * https://oeis.org/A362495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n} (C(n,m) - C(o,m) - C(e,m)) * B[n-m] * where
 *            o = (n+1)/2 (number of odd elements in [n]), * e = n/2 (number
 *            of even elements in [n]), * B[k] are Bell numbers (B[0]=1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 42,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(0)=a(1)=0 */

    /* Compute Bell numbers B[0..n] */
    seq_t *B = alloc_seq_array((size_t)(n + 1));
    B[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;                 /* C(i-1,0) */
        for (idx_t k = 0; k <= i - 1; ++k) {
            sum += bin * B[k];
            bin = bin * (seq_t)(i - 1 - k) / (k + 1);
        }
        B[i] = sum;
    }

    /* Compute a(n) using the formula */
    seq_t result = 0;
    seq_t bin_n = 1;   /* C(n,0)    */
    seq_t bin_o = 1;   /* C(o,0)    */
    seq_t bin_e = 1;   /* C(e,0)    */

    idx_t o = (n + 1) / 2;   /* number of odds */
    idx_t e = n / 2;         /* number of evens */

    for (idx_t m = 1; m <= n; ++m) {
        /* Update binomial coefficients */
        bin_n = bin_n * (seq_t)(n - m + 1) / m;

        if (m <= o)
            bin_o = bin_o * (seq_t)(o - m + 1) / m;
        else
            bin_o = 0;

        if (m <= e)
            bin_e = bin_e * (seq_t)(e - m + 1) / m;
        else
            bin_e = 0;

        seq_t coeff = bin_n - bin_o - bin_e;
        seq_t term = coeff * B[n - m];
        result += term;
    }

    free(B);
    return result;
}
