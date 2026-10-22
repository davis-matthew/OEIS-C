/*
 * A341168  Number of partitions of n into 10 distinct nonzero decimal
 *          palindromes.
 * https://oeis.org/A341168
 *
 * Offset:    56    (a(56) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 227, u16 n <= 588, u32 n <= 3040, u64 n <= 9711,
 *            u128 n <= 9711, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341168: Number of partitions of n into 10 distinct nonzero decimal palindromes.
   Computation is performed by a classical 0/1 knapsack DP over the set of palindromes ≤ n. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef MAX_AUTO_N
#define MAX_AUTO_N 20000          /* upper bound for n used in the tests */
#endif

/* Global table of all non‑zero decimal palindromes up to 10000.  */
/* 1‑digit (9), 2‑digit (9), 3‑digit (90), 4‑digit (90) → 198 numbers.  */
static seq_t palins[256];
static size_t palcnt = 0;
static bool palinit = false;

/* ----------------------------------------------------------------------- */
/* Build the list of palindromes once (up to 10000).  */
static void build_palins(void)
{
    size_t idx = 0;

    /* 1‑digit palindromes */
    for (int d = 1; d <= 9; ++d)
        palins[idx++] = (seq_t)d;

    /* 2‑digit palindromes (11, 22, …, 99) */
    for (int d = 1; d <= 9; ++d)
        palins[idx++] = (seq_t)(d * 10 + d);

    /* 3‑digit palindromes (101–999) */
    for (int a = 1; a <= 9; ++a)
        for (int b = 0; b <= 9; ++b)
            palins[idx++] = (seq_t)(a * 100 + b * 10 + a);

    /* 4‑digit palindromes (1001–9999) */
    for (int a = 1; a <= 9; ++a)
        for (int b = 0; b <= 9; ++b)
            palins[idx++] = (seq_t)(a * 1000 + b * 100 + b * 10 + a);

    palcnt = idx;
    palinit = true;
}

/* ----------------------------------------------------------------------- */
/* Count of partitions of n into exactly 10 distinct palindromes.  */
seq_t a(idx_t n)
{
    if (!palinit)      /* build the palindrome table once */
        build_palins();

    if (n > MAX_AUTO_N)   /* safety guard – not expected in tests */
        return 0;

    size_t N = (size_t)n;
    const size_t K = 10;
    const size_t SZ = (N + 1) * (K + 1);

    /* Allocate DP table: dp[s][k] → dp[s*(K+1)+k]  */
    seq_t *dp = (seq_t *)calloc(SZ, sizeof(seq_t));
    if (!dp) return 0;   /* allocation failure – unlikely */

    dp[0 * (K + 1) + 0] = (seq_t)1;   /* base: zero sum with zero parts */

    /* 0/1 knapsack over palindromes */
    for (size_t pi = 0; pi < palcnt; ++pi) {
        seq_t p = palins[pi];
        if (p > n) break;   /* all further palindromes are larger */

        /* s runs downward to enforce distinctness */
        for (size_t s = N; s >= (size_t)p; --s) {
            size_t base_s    = s * (K + 1);
            size_t base_sp   = (s - (size_t)p) * (K + 1);
            for (size_t k = K; k >= 1; --k) {
                dp[base_s + k] += dp[base_sp + (k - 1)];
            }
        }
    }

    seq_t result = dp[N * (K + 1) + K];
    free(dp);
    return result;
}
