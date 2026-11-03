/*
 * A249249  Total number of occurrences of the consecutive step pattern given
 *          by the binary expansion of n (where 1=up and 0=down) in all
 *          permutations of [n].
 * https://oeis.org/A249249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>0} k*A242783(n,k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 128 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements the definition by counting
 * the total number of occurrences of the binary pattern of n
 * in all permutations of [n].
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint64_t linear_extensions(idx_t pattern, idx_t r)
{
    /* r = length of the binary pattern (number of steps) */
    idx_t m = r + 1;                    /* number of vertices */
    size_t size = 1u << m;              /* number of subsets */
    static uint64_t dp[1 << 15];
    static idx_t pred[15];

    /* clear DP array */
    memset(dp, 0, size * sizeof(uint64_t));
    dp[0] = 1;

    /* clear predecessor masks */
    for (idx_t i = 0; i < m; ++i) pred[i] = 0;

    /* build predecessor masks from the pattern bits (MSB first) */
    for (idx_t j = 0; j < r; ++j) {
        bool up = ((pattern >> (r - 1 - j)) & 1ULL);
        if (up) {                      /* p[j] < p[j+1]  -> j before j+1 */
            pred[j + 1] |= (1ULL << j);
        } else {                       /* p[j] > p[j+1]  -> j+1 before j */
            pred[j]   |= (1ULL << (j + 1));
        }
    }

    /* DP over subsets to count linear extensions */
    for (size_t mask = 0; mask < size; ++mask) {
        if (dp[mask] == 0) continue;
        for (idx_t v = 0; v < m; ++v) {
            if (mask & (1ULL << v)) continue;              /* already placed */
            if ((pred[v] & ~mask) == 0) {                  /* all predecessors placed */
                dp[mask | (1ULL << v)] += dp[mask];
            }
        }
    }

    return dp[size - 1];
}

/* The build supplies the integer types seq_t and idx_t. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* compute binary length of n (number of bits) */
    idx_t r = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        ++r;
        tmp >>= 1;
    }
    if (r >= n) return 0;                    /* no possible window */

    /* number of linear extensions for the pattern of n */
    uint64_t Lp = linear_extensions(n, r);

    /* compute n! / (r+1)!  as product from (r+2) to n */
    seq_t prod = 1;
    for (idx_t i = r + 2; i <= n; ++i) {
        prod *= (seq_t)i;
    }

    /* final result: (n - r) * Lp * prod */
    seq_t result = prod * (seq_t)Lp;
    result *= (seq_t)(n - r);
    return result;
}
