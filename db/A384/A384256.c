/*
 * A384256  a(n) = Product_{k=0..n-1} (n+3*k+1).
 * https://oeis.org/A384256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0}^{n-1} (n + 3*k + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n = 0 the product is defined to be 1 */
    if (n == 0u) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    for (idx_t k = 0u; k < n; ++k) {
        /* term = n + 1 + 3*k, cast operands to seq_t before multiplying */
        seq_t term = (seq_t)n + (seq_t)1 + (seq_t)3 * (seq_t)k;
        result *= term;
    }

    return result;
}
