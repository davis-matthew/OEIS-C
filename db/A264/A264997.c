/*
 * A264997  Number of partitions of n into distinct parts of the form 3^a*5^b.
 * https://oeis.org/A264997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = number of partitions of n into distinct parts of the form
 *            3^a*5^b
 * Range:     u8 n <= 1334, u16 n <= 9281, u32 n <= 9281, u64 n <= 9281,
 *            u128 n <= 9281, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static table of computed values.  dp_len is the size of dp */
    static seq_t *dp = NULL;
    static idx_t dp_len = 0;

    /* If we already have the answer for this n, return it. */
    if (dp != NULL && n < dp_len)
        return dp[n];

    /* Allocate or grow the table so that it can hold indices up to n. */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(dp, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;
    dp = tmp;

    /* Zero all entries (including the new ones).  We will recompute everything. */
    for (idx_t i = 0; i < new_len; ++i)
        dp[i] = 0;
    dp[0] = 1;          /* the empty partition */

    /* Generate all numbers of the form 3^a * 5^b that are <= n. */
    size_t parts_cap = 64;
    size_t parts_len = 0;
    idx_t *parts = (idx_t *)malloc(parts_cap * sizeof(idx_t));
    if (!parts) return (seq_t)0;

    for (idx_t p3 = 1; p3 <= n; p3 *= 3) {
        for (idx_t p5 = 1; p5 <= n / p3; p5 *= 5) {
            idx_t val = p3 * p5;
            if (val > n) break;
            if (parts_len == parts_cap) {
                parts_cap <<= 1;
                idx_t *tmp2 = (idx_t *)realloc(parts, parts_cap * sizeof(idx_t));
                if (!tmp2) { free(parts); return (seq_t)0; }
                parts = tmp2;
            }
            parts[parts_len++] = val;
        }
    }

    /* Dynamic‑programming over the list of distinct parts. */
    for (size_t pi = 0; pi < parts_len; ++pi) {
        idx_t v = parts[pi];
        for (idx_t i = n; ; --i) {
            dp[i] += dp[i - v];
            if (i == v) break;          /* guard against unsigned underflow */
        }
    }

    free(parts);
    dp_len = new_len;
    return dp[n];
}
