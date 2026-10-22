/*
 * A330717  a(n) is the greatest binary palindrome of the form floor(n/2^k)
 *          with k >= 0.
 * https://oeis.org/A330717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   _is_binpal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) = the greatest binary palindrome of the form ⌊n/2ᵏ⌋,
 *   i.e. the longest binary-palindromic prefix of n.
 *   Implementation: repeatedly shift n right until a binary palindrome
 *   is found.  This uses only integer bitwise operations supported for
 *   the arbitrary‑precision type seq_t.
 */

#include <stdbool.h>
#include "../utils/_is_binpal.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    while (!_is_binpal(val))
        val >>= 1;
    return val;
}
