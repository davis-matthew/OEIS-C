/*
 * A383202  Number of positive integers with n digits in which adjacent digits
 *          differ by at most 7.
 * https://oeis.org/A383202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A383202:
 *  a(1) = 9
 *  For n ≥ 2, construct all n‑digit base‑10 numbers whose adjacent digits
 *  differ by at most 7.
 *  The value a(n) is the number of such n‑digit integers.
 *
 *  This implementation uses dynamic programming with memoisation.
 *  The state for length k is an array `dp[d]` storing the number of
 *  length‑k sequences ending with digit d (0 ≤ d ≤ 9).  Transition
 *  from length k to k+1 is:
 *
 *    next[d] = Σ_{p : |p‑d|≤7} dp[p]
 *
 *  The first digit cannot be 0, so for k = 1 we initialise
 *  `dp[0] = 0` and `dp[1…9] = 1`.
 *
 *  Results are cached in a static array `cache[1…n]` so that repeated
 *  calls with arbitrary n are O(n) overall.
 *
 *  The function works for any seq_t defined by the build
 *  (uint8_t…uint64_t, unsigned __int128, or an arbitrary‑precision
 *  integer class).  Only integer arithmetic is used, as required.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset < 1 */

    /* static memoisation state */
    static seq_t *cache = NULL;           /* cache[1..max] */
    static idx_t cache_len = 0;           /* current maximum precomputed n */
    static seq_t dp[10]   = {0};          /* counts for current length */
    static seq_t nextdp[10] = {0};

    /* initialise for the first time */
    if (!cache) {
        /* allocate space for indices 0..1 (ignore index 0) */
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        if (!cache) exit(1);
        cache[0] = (seq_t)0;             /* unused */

        /* length‑1: first digit cannot be 0 */
        for (int d = 0; d < 10; ++d) dp[d] = (seq_t)0;
        for (int d = 1; d < 10; ++d) dp[d] = (seq_t)1;

        /* store a(1) */
        seq_t total = (seq_t)0;
        for (int d = 0; d < 10; ++d) total += dp[d];
        cache[1] = total;
        cache_len = 1;
    }

    /* extend cache if required */
    if (n > cache_len) {
        size_t new_cap = (size_t)(n + 1);          /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;

        for (idx_t len = cache_len + 1; len <= n; ++len) {
            /* compute nextdp from current dp */
            for (int d = 0; d < 10; ++d) {
                seq_t sum = (seq_t)0;
                for (int p = 0; p < 10; ++p) {
                    int diff = (p > d) ? (p - d) : (d - p);
                    if ((unsigned)diff <= 7U) {
                        sum += dp[p];
                    }
                }
                nextdp[d] = sum;
            }
            /* total number of length‑len sequences */
            seq_t tot = (seq_t)0;
            for (int d = 0; d < 10; ++d) tot += nextdp[d];
            cache[len] = tot;

            /* prepare for next iteration */
            for (int d = 0; d < 10; ++d) dp[d] = nextdp[d];
        }

        cache_len = n;
    }

    return cache[n];
}
