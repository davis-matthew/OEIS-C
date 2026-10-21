/*
 * A256088  Non-palindromic balanced numbers in base 8.
 * https://oeis.org/A256088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1764, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth non‑palindromic balanced number in base 8 by
   brute‑forcing integers and testing the weighted digit sum.
   Balanced means \sum_{i} d_i*(i - (len-1)/2)=0, excluding palindromes. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;

    uint64_t x = 1;          /* candidate integer, in base 10 */
    idx_t  count = 0;        /* how many balanced numbers found so far */

    /* fixed maximum number of base‑8 digits needed for 64‑bit numbers */
    const size_t MAX_DIGITS = 32;
    uint8_t rev[MAX_DIGITS];    /* digits in reverse order (least significant first) */

    while (1) {
        /* extract base‑8 digits of x into rev[] */
        uint64_t t   = x;
        size_t   len = 0;
        while (t > 0) {
            rev[len++] = (uint8_t)(t % 8);
            t /= 8;
        }
        if (len == 0) {            /* should not happen, but keep it safe */
            rev[len++] = 0;
        }

        /* check whether the number is a palindrome in base 8 */
        bool pal = true;
        for (size_t i = 0; i < len / 2; i++) {
            if (rev[i] != rev[len - 1 - i]) { pal = false; break; }
        }

        /* if not a palindrome, test the balanced condition */
        if (!pal) {
            int64_t sum2 = 0;  /* twice the weighted sum, avoids fractions */
            for (size_t j = 0; j < len; ++j) {
                int64_t coeff = (int64_t)(2 * j - (int64_t)(len - 1));
                sum2 += coeff * (int64_t)rev[len - 1 - j];
            }
            if (sum2 == 0) {
                if (++count == n) {
                    return (seq_t)x;
                }
            }
        }
        ++x;
    }
}
