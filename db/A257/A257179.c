/*
 * A257179  Expansion of (1 + x^5) / ((1 - x) * (1 + x^4)) in powers of x.
 * https://oeis.org/A257179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 1,1,1,0,1,1,1,2 repeated with period
 *            8, i.e. a(n) = pattern[(n-1) % 8], where pattern =
 *            {1,1,1,0,1,1,1,2}
 * Range:     u8 n <= 9987, u16 n <= 9987, u32 n <= 9987, u64 n <= 9987,
 *            u128 n <= 9987, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    static const seq_t pattern[8] =
    {
        (seq_t)1, /* n ≡ 1 (mod 8) */
        (seq_t)1, /* n ≡ 2 (mod 8) */
        (seq_t)1, /* n ≡ 3 (mod 8) */
        (seq_t)0, /* n ≡ 4 (mod 8) */
        (seq_t)1, /* n ≡ 5 (mod 8) */
        (seq_t)1, /* n ≡ 6 (mod 8) */
        (seq_t)1, /* n ≡ 7 (mod 8) */
        (seq_t)2  /* n ≡ 0 (mod 8) and n > 0 */
    };

    /* (n-1) mod 8 using a bitwise mask (since 8 = 2^3) */
    size_t idx = ((size_t)(n - 1)) & 7UL;
    return pattern[idx];
}
