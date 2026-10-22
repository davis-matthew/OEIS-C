/*
 * A342123  a(n) is the remainder when n is divided by its binary reverse.
 * https://oeis.org/A342123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = n mod rev_bits(n) where rev_bits(n) is the binary *
 *            reverse of n as defined in A030101
 * Range:     u8 n <= 257, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   rev_bits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the exact definition and works for
 * any seq_t that supports + - * / % and bitwise operations. */

#include <stdint.h>
#include "../utils/rev_bits_idx.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
     * as a safe value that will never be used. */
    if (n == 0) return 0;
    seq_t rev = (seq_t)rev_bits_idx(n);
    return (seq_t)n % rev;
}
