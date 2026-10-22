/*
 * A341167  Number of partitions of n into 9 distinct nonzero decimal
 *          palindromes.
 * https://oeis.org/A341167
 *
 * Offset:    45    (a(45) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into 9 distinct nonzero decimal
 *            palindromes
 * Range:     u8 n <= 197, u16 n <= 601, u32 n <= 4722, u64 n <= 9838,
 *            u128 n <= 9838, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAXN 10000

static seq_t dp[10][MAXN + 1];
static bool init = false;

static void init_dp(void)
{
    idx_t pal[200];
    idx_t pc = 0;

    /* 1‑digit palindromes */
    for (idx_t d = 1; d <= 9; ++d)
        pal[pc++] = d;

    /* 2‑digit palindromes AA */
    for (idx_t d = 1; d <= 9; ++d)
        pal[pc++] = d * 11;

    /* 3‑digit palindromes ABA */
    for (idx_t d1 = 1; d1 <= 9; ++d1)
        for (idx_t d2 = 0; d2 <= 9; ++d2)
            pal[pc++] = d1 * 100 + d2 * 10 + d1;

    /* 4‑digit palindromes ABBA */
    for (idx_t d1 = 1; d1 <= 9; ++d1)
        for (idx_t d2 = 0; d2 <= 9; ++d2)
            pal[pc++] = d1 * 1000 + d2 * 100 + d2 * 10 + d1;

    /* initialise dp */
    for (idx_t k = 0; k <= 9; ++k)
        for (int s = 0; s <= MAXN; ++s)
            dp[k][s] = 0;
    dp[0][0] = 1;

    /* dynamic programming over palindromes */
    for (idx_t i = 0; i < pc; ++i) {
        idx_t p = pal[i];
        if (p > MAXN) continue;
        for (int k = 9; k >= 1; --k) {
            for (int s = MAXN; s >= (int)p; --s) {
                dp[k][s] += dp[k - 1][s - (int)p];
            }
        }
    }

    init = true;
}

seq_t a(idx_t n)
{
    if (n < 45) return 0;
    if (!init) init_dp();
    if (n > MAXN) return 0;
    return dp[9][(int)n];
}
