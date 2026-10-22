/*
 * A356920  Number of partitions of n into 6 parts that divide n.
 * https://oeis.org/A356920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 6 parts that all divide n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find all divisors of n */
    uint64_t nn = (uint64_t)n;
    uint64_t sqrt_n = 0;
    while ((sqrt_n + 1) * (sqrt_n + 1) <= nn) sqrt_n++;

    size_t cap = 2 * (size_t)sqrt_n + 2;
    uint64_t *divs = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t dcnt = 0;
    for (uint64_t i = 1; i <= sqrt_n; ++i) {
        if (nn % i == 0) {
            divs[dcnt++] = i;
            if (i != nn / i) divs[dcnt++] = nn / i;
        }
    }
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* DP arrays */
    size_t N = (size_t)n;
    seq_t **dp     = (seq_t **)malloc(7 * sizeof(seq_t *));
    seq_t **next   = (seq_t **)malloc(7 * sizeof(seq_t *));
    for (size_t k = 0; k <= 6; ++k) {
        dp[k]   = (seq_t *)calloc(N + 1, sizeof(seq_t));
        next[k] = (seq_t *)calloc(N + 1, sizeof(seq_t));
    }
    dp[0][0] = 1;   /* one way to choose 0 parts summing to 0 */

    for (size_t idx = 0; idx < dcnt; ++idx) {
        uint64_t d = divs[idx];
        /* Clear next array */
        for (size_t k = 0; k <= 6; ++k)
            memset(next[k], 0, (N + 1) * sizeof(seq_t));

        for (size_t k = 0; k <= 6; ++k) {
            for (size_t s = 0; s <= N; ++s) {
                seq_t val = dp[k][s];
                if (!val) continue;
                size_t max_t = 6 - k;
                for (size_t t = 0; t <= max_t; ++t) {
                    size_t ns = s + t * d;
                    if (ns > N) break;
                    next[k + t][ns] += val;
                }
            }
        }
        /* swap dp and next */
        seq_t **tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[6][N];

    /* Free memory */
    for (size_t k = 0; k <= 6; ++k) {
        free(dp[k]);
        free(next[k]);
    }
    free(dp);
    free(next);
    free(divs);

    return result;
}
