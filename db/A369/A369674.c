/*
 * A369674  a(n) = Product_{k=0..n} (3^k + 3^(n-k)).
 * https://oeis.org/A369674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (3^k + 3^(n-k))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 */
    if (n == 0) {
        return (seq_t)2;   /* 1 + 1 */
    }

    /* Allocate array for powers of 3 up to 3^n */
    seq_t *pow3 = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!pow3) {
        return (seq_t)0;   /* allocation failure, return 0 */
    }

    /* Compute powers: pow3[i] = 3^i */
    pow3[0] = (seq_t)1;
    const seq_t three = (seq_t)3;
    for (idx_t i = 1; i <= n; ++i) {
        pow3[i] = pow3[i - 1] * three;
    }

    /* Compute the product */
    seq_t result = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow3[k] + pow3[n - k];
        result *= term;
    }

    free(pow3);
    return result;
}
