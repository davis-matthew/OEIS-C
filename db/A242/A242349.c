/*
 * A242349  Largest power of 2 <= n^2.
 * https://oeis.org/A242349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^{⌊log₂(n²)⌋} (largest power of 2 ≤ n²)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   bitlen_seq_t, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;          /* compute n² in seq_t */
    unsigned len = bitlen_seq_t(sq);        /* number of bits of n²   */
    if (len == 0)               /* n=0 is outside the
                                 * sequence offset, but
                                 * handle it safely  */
        return 0;
    idx_t e = (idx_t)(len - 1);      /* exponent of the
                                     * largest power of two ≤ n² */
    return pow2(e);                  /* 2^e in seq_t   */
}
