/*
 * A331472  Consider the different ways to split the decimal representation of
 *          n into palindromic parts; a(n) is the greatest possible sum of the
 *          parts of such a split.
 * https://oeis.org/A331472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum sum of parts in a palindromic partition of the
 *            decimal representation of n (leading zeros in parts are allowed)
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

seq_t a(idx_t n) {
    // Special case: a(0) = 0
    if (n == 0) return (seq_t)0;

    /* Convert n to its decimal string representation without leading zeros. */
    char s[21];          // maximum 20 digits for 64‑bit unsigned
    int len = 0;
    idx_t temp = n;
    if (temp == 0) {
        s[0] = '0';
        len = 1;
    } else {
        /* Determine length. */
        while (temp > 0) {
            temp /= 10;
            ++len;
        }
        /* Fill the string from the end. */
        int pos = len - 1;
        temp = n;
        while (temp > 0) {
            s[pos--] = (char)('0' + (temp % 10));
            temp /= 10;
        }
    }

    /* Dynamic programming: dp[i] = maximum sum for the first i digits. */
    seq_t dp[21] = {0};
    dp[0] = 0;
    for (int i = 1; i <= len; ++i) {
        seq_t best = 0;
        for (int j = 0; j < i; ++j) {
            /* Check if substring s[j..i-1] is a palindrome. */
            int l = j, r = i - 1;
            bool pal = true;
            while (l < r) {
                if (s[l] != s[r]) { pal = false; break; }
                ++l; --r;
            }
            if (!pal) continue;

            /* Compute the integer value of the substring. */
            seq_t val = 0;
            for (int k = j; k < i; ++k) {
                val = val * (seq_t)10 + (seq_t)(s[k] - '0');
            }
            seq_t candidate = dp[j] + val;
            if (candidate > best) best = candidate;
        }
        dp[i] = best;
    }
    return dp[len];
}
