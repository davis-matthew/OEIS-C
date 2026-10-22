/*
 * A319440  Squares of non-palindromic number.
 * https://oeis.org/A319440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (nth non‑palindromic integer)^2
 * Range:     u8 n <= 5, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num   = 0;
    while (count < n) {
        ++num;
        if (!is_palindrome((seq_t)num, (idx_t)10))
            ++count;
    }
    seq_t val = (seq_t)num;
    return val * val;
}
