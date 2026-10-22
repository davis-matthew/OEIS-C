/*
 * A341453  Number of partitions of n into 6 nonprime parts.
 * https://oeis.org/A341453
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into exactly 6 nonprime parts
 *            (i.e., parts that are 1 or composite). The routine builds the
 *            table of partitions in nondecreasing order of part size, so each
 *            unordered partition is counted once
 * Range:     u8 n <= 47, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* dp[k][s] = number of partitions of s into k allowed parts
   allowed parts are 1 or composite numbers.  */
static seq_t *dp[7] = {0};
static idx_t max_n = 0;          /* largest n for which dp is valid */

static void ensure_dp(idx_t n)
{
    if (n <= max_n) return;     /* already computed up to this n */

    /* Free any previous tables and allocate new ones */
    for (int k = 0; k <= 6; ++k) {
        free(dp[k]);
        dp[k] = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
        if (!dp[k]) exit(1);
    }

    /* Sieve to know which numbers are composite (or 1) */
    unsigned char *composite = (unsigned char *)calloc((size_t)n + 1, sizeof(unsigned char));
    if (!composite) exit(1);
    composite[0] = composite[1] = 1;            /* 0 and 1 treated as allowed */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                composite[j] = 1;
        }
    }

    /* Base case: one way to partition 0 into 0 parts */
    dp[0][0] = 1;

    /* DP: iterate over each allowed part in increasing order */
    for (idx_t p = 1; p <= n; ++p) {
        if (p == 1 || composite[p]) {        /* allowed part */
            for (int k = 1; k <= 6; ++k) {
                for (idx_t s = p; s <= n; ++s) {
                    dp[k][s] += dp[k - 1][s - p];
                }
            }
        }
    }

    free(composite);
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 6) return 0;          /* values below the offset are undefined */
    ensure_dp(n);
    return dp[6][n];
}
