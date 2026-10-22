/*
 * A319475  Number of partitions of n into exactly ten nonzero decimal
 *          palindromes.
 * https://oeis.org/A319475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 234, u32 n <= 2526, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of partitions of n into exactly ten nonzero decimal palindromes. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* For n < 1 there is no way to partition into ten positive palindromes. */
    if (n < 1) return 0;

    /* ------------------------------------------------------------------
       Generate all non‑zero decimal palindromes <= n.
       ------------------------------------------------------------------ */
    idx_t *pal = NULL;          /* array of palindromes */
    size_t pal_cap = 0;         /* current capacity of pal array */
    size_t pal_cnt = 0;         /* number of palindromes stored */

    /* Determine the number of decimal digits in n. */
    int max_len = 0;
    {
        idx_t tmp = n;
        do {
            ++max_len;
            tmp /= 10U;
        } while (tmp);
    }

    /* Pre‑compute powers of ten up to max_len. */
    idx_t pow10[21];
    pow10[0] = 1U;
    for (int i = 1; i <= max_len; ++i) {
        pow10[i] = pow10[i-1] * 10U;
    }

    /* Enumerate palindromes of each possible length. */
    for (int len = 1; len <= max_len; ++len) {
        int half = (len + 1) / 2;                 /* digits in the left half */
        idx_t start = pow10[half-1];              /* first half cannot start with 0 */
        idx_t end   = pow10[half] - 1U;           /* last half */

        for (idx_t pref = start; pref <= end; ++pref) {
            /* Build the palindrome from prefix. */
            idx_t rev = 0;
            idx_t tmp = pref;
            int steps = len / 2;
            if (len & 1) {          /* odd length: ignore middle digit when reversing */
                tmp /= 10U;
            }
            for (int i = 0; i < steps; ++i) {
                rev = rev * 10U + tmp % 10U;
                tmp /= 10U;
            }
            idx_t pal_val = pref * pow10[steps] + rev;
            if (pal_val > n) continue;          /* only keep palindromes <= n */

            /* Store palindrome, growing array if necessary. */
            if (pal_cnt == pal_cap) {
                pal_cap = pal_cap ? pal_cap * 2 : 8;
                idx_t *tmp = (idx_t *)realloc(pal, pal_cap * sizeof(idx_t));
                if (!tmp) {
                    free(pal);
                    return 0;          /* allocation failure: return 0 as safe value */
                }
                pal = tmp;
            }
            pal[pal_cnt++] = pal_val;
        }
    }

    /* ------------------------------------------------------------------
       Count partitions using dynamic programming.
       ------------------------------------------------------------------ */
    const int PARTS = 10;
    size_t rows = (size_t)n + 1;
    size_t cols = PARTS + 1;
    size_t total = rows * cols;

    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));   /* zero‑initialised */
    if (!dp) {
        free(pal);
        return 0;
    }
    dp[0] = 1;          /* dp[0][0] = 1 */

    for (size_t pi = 0; pi < pal_cnt; ++pi) {
        idx_t p = pal[pi];
        for (idx_t s = p; s <= n; ++s) {
            size_t cur = (size_t)s * cols;
            size_t prev = (size_t)(s - p) * cols;
            for (int k = 1; k <= PARTS; ++k) {
                dp[cur + k] += dp[prev + (k - 1)];
            }
            if (s == n) break;   /* avoid unsigned overflow when s == UINT64_MAX */
        }
    }

    seq_t result = dp[(size_t)n * cols + PARTS];

    free(dp);
    free(pal);
    return result;
}
