/*
 * A261787  a(n) is the smallest nonzero number that is not a substring of n
 *          in ternary representation.
 * https://oeis.org/A261787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261787: a(n) = smallest positive integer whose ternary representation
   is not a substring of n's ternary representation. */

#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Convert n to ternary string s[0..len-1] (most significant first). */
    char s[64];
    idx_t len = 0;
    idx_t tmp = n;
    if (tmp == 0) {
        s[len++] = '0';
    } else {
        while (tmp > 0) {
            s[len++] = '0' + (tmp % 3);
            tmp /= 3;
        }
        /* Reverse to make most significant digit first. */
        for (idx_t i = 0; i < len / 2; ++i) {
            char c = s[i];
            s[i] = s[len - 1 - i];
            s[len - 1 - i] = c;
        }
    }

    /* Search for the smallest k whose ternary representation is NOT a substring of s. */
    for (idx_t k = 1; ; ++k) {
        /* Convert k to ternary string t[0..len_t-1]. */
        char t[64];
        idx_t len_t = 0;
        idx_t tmp2 = k;
        if (tmp2 == 0) {
            t[len_t++] = '0';
        } else {
            while (tmp2 > 0) {
                t[len_t++] = '0' + (tmp2 % 3);
                tmp2 /= 3;
            }
            for (idx_t i = 0; i < len_t / 2; ++i) {
                char c = t[i];
                t[i] = t[len_t - 1 - i];
                t[len_t - 1 - i] = c;
            }
        }

        /* If the candidate's length exceeds the target's length, it cannot be a substring. */
        if (len_t > len) {
            return (seq_t)k;
        }

        /* Scan for substring occurrence. */
        int found = 0;
        for (idx_t i = 0; i + len_t <= len; ++i) {
            if (memcmp(s + i, t, (size_t)len_t) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return (seq_t)k;
        }
    }
}
