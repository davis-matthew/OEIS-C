/*
 * A373078  Number of partitions of n into distinct parts such that number of
 *          parts is a multiples of 5.
 * https://oeis.org/A373078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A373078 – partitions of n into distinct parts with number of
 *            parts a multiple of 5 (i.e. parts count ≡ 0 mod 5). Enumerated
 *            by the product ∏_{i≥1} (1+x^i) and selecting terms with the
 *            exponent of the parts count congruent to 0 modulo 5
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *dp = NULL;          /* flattened array: dp[sum*5 + r] */
static idx_t  max_n = 0;          /* largest n for which dp is valid */
static size_t dp_cap = 0;         /* allocated entries (max_n+1)*5 */

static void recompute(idx_t n)
{
    size_t new_cap = (size_t)(n + 1) * 5;
    if (dp == NULL) {
        dp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!dp) exit(1);
    } else if (dp_cap < new_cap) {
        dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!dp) exit(1);
    }
    /* zero‑initialize the whole table */
    for (size_t i = 0; i < new_cap; ++i) dp[i] = 0;
    dp[0] = 1;                       /* dp[0][0] = 1, others 0 */

    for (idx_t part = 1; part <= n; ++part) {
        for (idx_t sum = n; sum >= part; --sum) {
            size_t src_base = (size_t)(sum - part) * 5;
            size_t dst_base = (size_t)sum * 5;
            for (idx_t r = 0; r < 5; ++r) {
                seq_t add = dp[src_base + r];
                if (add) {
                    dp[dst_base + ((r + 1) % 5)] += add;
                }
            }
        }
    }
    max_n = n;
    dp_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (dp == NULL || n > max_n) recompute(n);
    return dp[(size_t)n * 5 + 0];
}
