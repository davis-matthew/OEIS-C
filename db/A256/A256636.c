/*
 * A256636  Expansion of phi(-x^3) / f(-x^2) in powers of x where phi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A256636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Coefficient of x^n in * ∏_{k>0} (1 + x^k + x^(2k)) / ((1
 *            + x^k)·(1 + x^(3k)))
 * Range:     i8 n <= 25, i16 n <= 73, i32 n <= 228, i64 n <= 776,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS sequence A256636. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;  /* a(0)=1 */

    /* Allocate an array of coefficients up to index n. */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;           /* allocation failure – should not happen */

    /* Initialize coefficients: dp[0] = 1, others = 0. */
    for (size_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Part 1: multiply by ∏_{k≥1} 1/(1 - x^{6k-2}) and ∏_{k≥1} 1/(1 - x^{6k-4}) */
    for (size_t s = 2; s <= N; s += 6)          /* 2, 8, 14, …  (≡2 mod 6) */
        for (size_t i = s; i <= N; ++i)
            dp[i] += dp[i - s];

    for (size_t s = 4; s <= N; s += 6)          /* 4, 10, 16, …  (≡4 mod 6) */
        for (size_t i = s; i <= N; ++i)
            dp[i] += dp[i - s];

    /* Part 2: multiply by ∏_{k≥1} (1 - 2x^{6k-3} + x^{2(6k-3)}) */
    for (size_t s = 3; s <= N; s += 6)          /* 3, 9, 15, …  (≡3 mod 6) */
    {
        for (size_t i = N; i >= s; --i)        /* descending to use old values only */
        {
            seq_t old_val   = dp[i];
            seq_t sub_val   = dp[i - s];
            seq_t add_val   = (i >= 2 * s) ? dp[i - 2 * s] : 0;
            dp[i] = old_val - (seq_t)2 * sub_val + add_val;
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
