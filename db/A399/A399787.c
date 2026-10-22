/*
 * A399787  Length of the longest palindromic substring of n written in base
 *          10.
 * https://oeis.org/A399787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest palindromic substring of the
 *            decimal representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the length of the longest palindromic substring of n written in base 10.
   n is given as an idx_t (uint64_t).  The value is returned as seq_t. */
seq_t a(idx_t n)
{
    /* Step 1: Convert n to a decimal digit array (in forward order). */
    uint8_t digits[64];
    size_t len = 0;
    uint64_t temp = n;
    if (temp == 0) {
        digits[0] = '0';
        len = 1;
    } else {
        while (temp > 0) {
            digits[len++] = (uint8_t)('0' + (temp % 10));
            temp /= 10;
        }
        /* reverse to obtain forward order */
        for (size_t i = 0; i < len / 2; ++i) {
            uint8_t t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* Step 2: Brute‑force search of all substrings, O(len³) but len ≤ 20 for uint64_t. */
    size_t maxlen = 1; /* every number has at least one digit */
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i; j < len; ++j) {
            size_t candlen = j - i + 1;
            if (candlen <= maxlen) continue;  /* prune obvious failures */
            size_t l = i, r = j;
            bool ispal = true;
            while (l < r) {
                if (digits[l] != digits[r]) {
                    ispal = false;
                    break;
                }
                ++l; --r;
            }
            if (ispal) maxlen = candlen;
        }
    }

    return (seq_t)maxlen;
}
