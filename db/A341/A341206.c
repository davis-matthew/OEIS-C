/*
 * A341206  Number of ways to write n as an ordered sum of 9 nonzero decimal
 *          palindromes.
 * https://oeis.org/A341206
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of ordered sums of 9 nonzero decimal palindromes
 *            (OEIS A341206). The routine generates all decimal palindromes ≤
 *            n (at most ~200 for n ≤ 10000) and uses dynamic programming with
 *            10 layers (k = 0…9) to accumulate the convolution. An internal
 *            cache holds the DP tables for the largest...
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 286, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* If the build supplies a helper is_palindrome, use it; otherwise
   provide a minimal local implementation (decimal base only). */
#ifndef NO_IS_PALINDROME
#include "../utils/is_palindrome.h"
#include "../utils/alloc_seq_array.h"
#else
/* Very small helper: check if seq_t value (treated as unsigned) is a
   palindrome in base 10. */
static bool is_palindrome(seq_t x, idx_t base) {
    (void)base;          /* ignore base, only decimal used */
    uint64_t n = (uint64_t)x;
    uint64_t rev = 0, temp = n;
    while (temp) { rev = rev * 10 + temp % 10; temp /= 10; }
    return rev == n;
}
#endif

/* Allocate an array of seq_t values, zeroed. */

/* Static state for memoisation. */
static idx_t max_n = 0;          /* now‑maximal n requested   */
static idx_t *pal = NULL;        /* list of palindromes ≤ max_n */
static size_t pal_cnt = 0;       /* number of palindromes      */

static seq_t *dp[10] = {NULL};   /* DP tables: dp[k][s]  */
static size_t dp_len = 0;        /* number of entries in each dp layer (max_n+1) */

/* Rebuild all tables when the required maximum n increases. */
static void recompute(idx_t n) {
    /* --- 1. regenerate palindromes up to n --------------------------------- */
    if (pal) free(pal);
    pal = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    pal_cnt = 0;
    for (idx_t v = 1; v <= n; ++v)
        if (is_palindrome((seq_t)v, 10))
            pal[pal_cnt++] = v;

    /* --- 2. rebuild DP arrays -------------------------------------------- */
    for (int k = 0; k <= 9; ++k) {
        if (dp[k]) free(dp[k]);            /* old array */
        dp[k] = alloc_seq_array((size_t)n + 1); /* new, zeroed */
    }

    dp[0][0] = 1;                          /* base case */

    for (int k = 1; k <= 9; ++k) {
        for (size_t s = 1; s <= (size_t)n; ++s) {
            seq_t sum = 0;
            for (size_t i = 0; i < pal_cnt; ++i) {
                idx_t p = pal[i];
                if ((idx_t)s < p) break;
                sum += dp[k-1][(size_t)((idx_t)s - p)];
            }
            dp[k][s] = sum;
        }
    }
    dp_len = (size_t)n + 1;
    max_n = n;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 9) return 0;                 /* offset: undefined below 9 */
    /* If a larger n is requested, rebuild the tables. */
    if ((size_t)n + 1 > dp_len)
        recompute(n);
    return dp[9][(size_t)n];
}
