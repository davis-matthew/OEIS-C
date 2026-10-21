/*
 * A258745  Order of general affine group AGL(n,2) (=A028365(n)) divided by
 *          (n+1).
 * https://oeis.org/A258745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{n(n+1)/2} * ∏_{i=1}^{n} (2^i - 1) / (n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = 2^{n(n+1)/2} * ∏_{i=1}^{n} (2^i - 1) / (n+1) */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 1;          /* current product 2^{i} * (2^i - 1) ...      */
    seq_t pow2   = (seq_t)2;   /* 2^i for the current i (starts with i=1)    */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Multiply by (2^i - 1) and then by 2^i itself */
        result = result * (pow2 - (seq_t)1);
        result = result * pow2;

        /* Prepare pow2 for the next i: 2^(i+1) */
        pow2 = pow2 * (seq_t)2;
    }

    /* Divide by (n+1) to complete the formula */
    result = result / ((seq_t)(n + 1));

    return result;
}
