/*
 * A376127  a(n) = 1 + Sum_{k=0..n-1} (k+1)^3 * a(k) * a(n-k-1).
 * https://oeis.org/A376127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} (k+1)^3 * a(k) * a(n-k-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array to hold values up to index n */
    seq_t *arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0;           /* safeguard, should not happen */

    arr[0] = 1;                          /* base value a(0) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t kp1 = (seq_t)(k + 1);       /* (k+1) */
            seq_t w = kp1 * kp1 * kp1;        /* (k+1)^3   */
            w = w * arr[k];                  /* weight * a(k)   */
            sum = sum + w * arr[i - 1 - k];   /* accumulate term */
        }
        arr[i] = sum + 1;                   /* a(i) = 1 + sum */
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
