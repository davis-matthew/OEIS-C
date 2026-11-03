/*
 * A341191  Number of ways to write n as an ordered sum of 2 nonzero decimal
 *          palindromes.
 * https://oeis.org/A341191
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ (p,q) | p>0, q>0, is_palindrome(p,10),
 *            is_palindrome(q,10), p+q=n }
 * Range:     u8 n <= 533, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 105 terms through n = 8843 (2026-11-01)
 * Helpers:   is_palindrome_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome_base.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* offset starts at 2, values below are irrelevant */

    seq_t count = 0;
    for (idx_t p = 1; p < n; ++p) {
        idx_t q = n - p;
        if (is_palindrome_base((seq_t)p, 10) && is_palindrome_base((seq_t)q, 10))
            ++count;
    }
    return count;
}
