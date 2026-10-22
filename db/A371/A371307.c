/*
 * A371307  The smallest nonpalindromic number in base n that shares the same
 *          prime divisors as its digit reversal in base n.
 * https://oeis.org/A371307
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest non‑palindromic number in base n that shares the
 *            same set of prime divisors as its digit reversal in base n
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   is_palindrome, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/rad.h"
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n < 2 */

    const idx_t base = n;
    uint64_t x = 1;

    while (1) {
        /* Skip palindromes in base n */
        if (is_palindrome((seq_t)x, base)) {
            x++;
            continue;
        }

        /* Compute the digit reversal of x in base n */
        uint64_t t = x;
        uint64_t rev = 0;
        while (t > 0) {
            uint64_t digit = t % base;
            /* protect against 64‑bit overflow; numbers that overflow are
               unlikely to be the minimal solution, so break the loop if
               it occurs. */
            if (rev > ULLONG_MAX / base) break;
            rev = rev * base + digit;
            t /= base;
        }
        if (t > 0) {             /* overflow happened, continue searching */
            x++;
            continue;
        }

        /* Compare rad(x) and rad(rev) – equal iff the two numbers have
           exactly the same prime divisors. */
        if (rad((idx_t)x) == rad((idx_t)rev))
            return (seq_t)x;

        x++;
    }
}
