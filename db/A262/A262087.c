/*
 * A262087  Largest palindrome p such that n-p is again a palindrome, or 0 if
 *          no such p exists.
 * https://oeis.org/A262087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest palindrome p ≤ n such that n−p is also a
 *            palindrome
 * Range:     u8 n <= 260, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9856 (2026-11-01)
 * Helpers:   is_palindrome_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_palindrome_base.h"

seq_t a(idx_t n)
{
    for (idx_t p = n; ; --p) {
        seq_t sp = (seq_t)p;
        if (is_palindrome_base(sp, 10)) {
            seq_t diff = (seq_t)(n - p);
            if (is_palindrome_base(diff, 10))
                return sp;
        }
        if (p == 0) break;
    }
    return 0;
}
