/*
 * A265573  Smallest base-9 palindrome m >= n, written in base 9.
 * https://oeis.org/A265573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least m >= n with a base‑9 palindromic representation;
 *            the result is the decimal integer formed by those base‑9 digits
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    seq_t m = n;
    /* Find the smallest base‑9 palindrome ≥ n. */
    while (!is_palindrome(m, 9))
        ++m;

    /* Convert m's base‑9 digits into a decimal integer. */
    seq_t r = 0;
    seq_t t = m;
    while (t > 0) {
        r = r * 10 + (t % 9);
        t /= 9;
    }
    return r;
}
