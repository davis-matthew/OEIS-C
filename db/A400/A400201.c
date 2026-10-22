/*
 * A400201  Palindrome with an odd number of digits, based on a sequence of
 *          odd digits.
 * https://oeis.org/A400201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Algorithm: the n-th term is the palindrome of length 2*n‑1 formed by the first
   (n‑1) digits of the repeating odd‑digit cycle 1,3,5,7,9, then the nth odd
   digit, followed by the reverse of the first (n‑1) digits. */
seq_t a(idx_t n)
{
    const uint8_t odds[5] = {1, 3, 5, 7, 9};
    seq_t res   = 0;
    idx_t left  = n - 1;          /* number of digits before the centre */

    for (idx_t i = 0; i < left; ++i)
        res = res * 10 + odds[i % 5];

    res = res * 10 + odds[left % 5];

    for (idx_t i = left; i > 0; --i)
        res = res * 10 + odds[(i - 1) % 5];

    return res;
}
