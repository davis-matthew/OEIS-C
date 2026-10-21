/*
 * A241766  Number of partitions of n into parts of the form 3^k - 2^k, cf.
 *          A001047.
 * https://oeis.org/A241766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts of the form 3^k -
 *            2^k (k ≥ 1)
 * Range:     u8 n <= 168, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n == 0: empty partition */
    if (n == 0)
        return (seq_t)1;

    /* collect all admissible parts up to n */
    size_t parts_cap = 8;
    size_t parts_len = 0;
    idx_t *parts = (idx_t *)malloc(parts_cap * sizeof(idx_t));
    if (!parts)
        return (seq_t)0; /* allocation failure */

    idx_t p3 = 3; /* 3^1 */
    idx_t p2 = 2; /* 2^1 */
    while (1) {
        idx_t part = p3 - p2;
        if (part > n)
            break;
        if (parts_len == parts_cap) {
            parts_cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(parts, parts_cap * sizeof(idx_t));
            if (!tmp) {
                free(parts);
                return (seq_t)0;
            }
            parts = tmp;
        }
        parts[parts_len++] = part;
        p3 *= 3;
        p2 *= 2;
    }

    /* dynamic programming array for partitions */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp) {
        free(parts);
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;

    for (size_t pi = 0; pi < parts_len; ++pi) {
        size_t part_sz = (size_t)parts[pi];
        for (size_t i = part_sz; i < sz; ++i) {
            dp[i] += dp[i - part_sz];
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(parts);
    return result;
}
