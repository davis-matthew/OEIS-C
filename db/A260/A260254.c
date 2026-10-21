/*
 * A260254  Number of ways to write n as sum of two palindromes in decimal
 *          representation.
 * https://oeis.org/A260254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number of unordered representations n = p + q with p and
 *            q being decimal palindromes (including 0)
 * Range:     u8 n <= 1420, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t p = 0; p <= n; ++p)
    {
        if (!is_palindrome(p, 10))
            continue;
        idx_t q = n - p;
        if (q < p)
            continue;
        if (is_palindrome(q, 10))
            ++count;
    }
    return count;
}
