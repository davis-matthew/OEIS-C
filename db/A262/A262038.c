/*
 * A262038  Least palindrome >= n.
 * https://oeis.org/A262038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least palindrome ≥ n (base 10)
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    /* 0 is a palindrome, so the loop will not run for n == 0 */
    while (!is_palindrome(cur, (idx_t)10))
        cur += (seq_t)1;
    return cur;
}
