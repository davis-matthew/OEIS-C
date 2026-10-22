/*
 * A336173  The Mignosi-Restivo repetition length for the Thue-Morse sequence
 *          (A010060) at position n.
 * https://oeis.org/A336173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0 a(n) = 3 * 2^⌊log₂(n)⌋ if n is odd a(n) = 1 if
 *            v₂(n) is odd otherwise a(n) = 2 if v₂(n) is even (n even) (v₂(n)
 *            = exponent of 2 in n, i.e., number of trailing zeros in n)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset 0 */
    if (n == 0)
        return (seq_t)1;

    /* odd indices */
    if (n & 1ULL)
    {
        /* floor(log2(n)) = position of highest 1-bit */
        idx_t high_bit = 63 - __builtin_clzll(n);          /* n > 0 */
        idx_t pow2 = 1ULL << high_bit;                    /* 2^⌊log₂(n)⌋ */
        return (seq_t)3 * (seq_t)pow2;
    }

    /* even indices */
    /* trailing-zero count gives exponent of 2 */
    idx_t tz = __builtin_ctzll(n);                        /* n > 0 */
    return (tz & 1) ? (seq_t)1 : (seq_t)2;
}
