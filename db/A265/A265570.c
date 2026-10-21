/*
 * A265570  Smallest base-8 palindrome m >= n, written in base 10.
 * https://oeis.org/A265570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest base-8 palindrome >= n (in base 10) */
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    while (!is_palindrome(m, (idx_t)8))
        ++m;
    return m;
}
