/*
 * A316936  a(n) is the maximum state complexity of the language C(w) of
 *          conjugates of w, over all length-n binary strings w.
 * https://oeis.org/A316936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{2i} + (2j-2i+3)2^i + j^2 - (2i+1)j - 1 * where n = 2^i
 *            + j and 0 <= j < 2^i. * This implements the OEIS definition of
 *            A316936
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we return 0
       (undefined but harmless) */
    if (n < 1)
        return (seq_t)0;

    /* Find i such that 2^i <= n < 2^{i+1}.  i is the floor of log2(n). */
    idx_t i = 0;
    idx_t pow_idx = 1;            /* 2^i in idx_t */
    while (pow_idx <= (n >> 1)) { /* avoid overflow: pow_idx <= n/2  */
        pow_idx <<= 1;
        ++i;
    }

    /* Now pow_idx = 2^i, j = n - pow_idx */
    idx_t j_idx = n - pow_idx;
    seq_t j = (seq_t)j_idx;

    /* Convert to seq_t for all arithmetic */
    seq_t two_i = ((seq_t)1) << i;           /* 2^i in seq_t */
    seq_t two_2i = two_i * two_i;            /* 2^{2i} */

    /* Temporary terms for the formula */
    seq_t two = (seq_t)2;
    seq_t i_seq = (seq_t)i;

    /* (2j - 2i + 3) * 2^i */
    seq_t term1 = (two * j - two * i_seq + (seq_t)3) * two_i;

    /* j^2 */
    seq_t term2 = j * j;

    /* (2i + 1) * j */
    seq_t term3 = (two * i_seq + (seq_t)1) * j;

    /* a(n) = 2^{2i} + term1 + term2 - term3 - 1 */
    seq_t result = two_2i + term1 + term2 - term3 - (seq_t)1;

    return result;
}
