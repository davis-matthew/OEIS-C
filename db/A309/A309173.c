/*
 * A309173  Expansion of Product_{k>=1} (1 + (1 + x + x^2) * x^k).
 * https://oeis.org/A309173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + (1 + x + x^2) · x^k)
 * Range:     u8 n <= 13, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficient counts the number of representations of n as a sum of a
   subset of the multiset {k, k+1, k+2 | k ≥ 1} with each k used at most once. */

#include <stdlib.h>   /* calloc, free */

/* Build supplies the types seq_t (value type) and idx_t (index type). */

seq_t a(idx_t n)
{
    /* Handle the offset 0 explicitly. */
    if (n == 0) return (seq_t)1;

    /* Convert the index to a size_t; the tests use values that fit. */
    size_t N = (size_t)n;

    /* dp[s] will hold the number of ways to reach sum s using
       the first few factors.  Use 0‑based indexing. */
    seq_t *dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* allocation failure – safe fallback */

    dp[0] = (seq_t)1;          /* there is one way to make sum 0: take nothing */

    /* Process each factor k = 1 .. N. */
    for (size_t k = 1; k <= N; ++k) {
        /* Update dp in reverse so each factor is used at most once. */
        for (size_t s = N; ; --s) {
            seq_t ways = dp[s];
            if (ways) {
                if (s + k       <= N) dp[s + k]       += ways;
                if (s + k + 1   <= N) dp[s + k + 1]   += ways;
                if (s + k + 2   <= N) dp[s + k + 2]   += ways;
            }
            if (s == 0) break;   /* size_t underflow would occur after 0 */
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
