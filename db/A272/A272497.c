/*
 * A272497  Number of ordered set partitions of [n] with nondecreasing block
 *          sizes and maximal block size equal to seven.
 * https://oeis.org/A272497
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of ordered set partitions of [n] with
 *            nondecreasing block sizes and maximal block size equal to seven
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t fact[8];                 /* factorials 0!…7! */
static bool facts_initialized = false;

/* multiplicative binomial coefficient C(n,k) for k<=7 */
static seq_t binom_small(idx_t n, uint8_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    seq_t num = 1;
    for (uint8_t t = 1; t <= k; ++t)
        num = num * (seq_t)(n - t + 1);
    return num / fact[k];
}

/*  DP table: dp[row][m] = number of ordered partitions of [row] with all block sizes <= m (m=0..7). */
static seq_t *dp = NULL;      /* flat array of size (max_n+1)*8 */
static idx_t max_n = 0;       /* highest n for which dp is computed */

/* Ensure dp is computed up to n inclusive.  Size grows only upwards. */
static void ensure_dp(idx_t n)
{
    if (n <= max_n) return;

    /* Allocate or enlarge the table. */
    size_t new_cells = (size_t)(n + 1) * 8;
    size_t old_cells = (size_t)(max_n + 1) * 8;
    dp = (seq_t *)realloc(dp, new_cells * sizeof(seq_t));
    if (!dp) exit(1);          /* out of memory */

    /* Initialize newly allocated part to zero. */
    for (size_t i = old_cells; i < new_cells; ++i)
        dp[i] = 0;

    /* Initialize base row (n = 0). */
    for (int m = 0; m <= 7; ++m)
        dp[0 * 8 + m] = 1;     /* empty partition */

    /* Compute rows from max_n+1 up to n. */
    for (idx_t row = max_n + 1; row <= n; ++row) {
        dp[row * 8 + 0] = 0;   /* no partitions with max size 0 */
        for (int m = 1; m <= 7; ++m) {
            seq_t sum = 0;
            int max_i = (row < m) ? (int)row : m;
            for (int i = 1; i <= max_i; ++i) {
                seq_t part = dp[(row - i) * 8 + i];
                seq_t b = binom_small(row, i);
                sum = sum + part * b;
            }
            dp[row * 8 + m] = sum;
        }
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    /* Only defined for n >= 7; below that we may return 0. */
    if (n < 7) return 0;

    if (!facts_initialized) {
        fact[0] = 1;
        for (int i = 1; i <= 7; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;
        facts_initialized = true;
    }

    ensure_dp(n);

    /* a(n) = dp[n][7] - dp[n][6] (max size exactly 7). */
    seq_t res = dp[n * 8 + 7] - dp[n * 8 + 6];
    return res;
}
