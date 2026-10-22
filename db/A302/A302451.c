/*
 * A302451  a(n) = [x^n] Product_{k>=0} (1 + n*x^(2^k)).
 * https://oeis.org/A302451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^A000120(n) (n raised to the number of 1-bits in n)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case: 0^0 is defined as 1 in this sequence */
    if (n == 0)
        return (seq_t)1;

    /* Count the 1-bits in the binary representation of n */
    uint64_t ones = __builtin_popcountll((uint64_t)n);

    /* Compute n^ones using the big_pow helper (handles exact arithmetic for all
       supported seq_t sizes, including arbitrary-precision). */
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)ones;
    return big_pow(base, exp);
}
