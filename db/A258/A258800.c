/*
 * A258800  The number of zeroless decimal numbers whose digital sum is n.
 * https://oeis.org/A258800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0 and for n>0, a(n) = ∑_{k=1}^{9} a(n-k), where a(0)=1 is
 *            the number of empty compositions. This is the number of zeroless
 *            decimal numbers whose digital sum is n
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free, exit */
#include <stdint.h>   /* uint64_t */

static seq_t *dp = NULL;       /* DP table: dp[i] is the number of
                                compositions of i into parts 1..9 */
static size_t dp_cap = 0;      /* allocated capacity */
static idx_t  computed_max = 0;/* highest index already computed */

/* Grow dp to hold values up to at least 'upto' (inclusive) */
static void grow_dp(idx_t upto)
{
    if (dp_cap > (size_t)upto) return;

    size_t new_cap = dp_cap ? dp_cap : 16;
    while (new_cap <= (size_t)upto) new_cap <<= 1;

    seq_t *new_dp;
#if defined(__cplusplus)
    new_dp = new seq_t[new_cap];
#else
    new_dp = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_dp) exit(EXIT_FAILURE);

    /* copy old data */
    if (dp) {
        for (size_t i = 0; i < dp_cap; ++i) new_dp[i] = dp[i];
#if defined(__cplusplus)
        delete[] dp;
#else
        free(dp);
#endif
    }

    /* zero-initialize new slots */
    for (size_t i = dp_cap; i < new_cap; ++i) new_dp[i] = (seq_t)0;

    dp = new_dp;
    dp_cap = new_cap;

    /* initialise base case if this is the first allocation */
    if (dp_cap > 0 && dp[0] == (seq_t)0) dp[0] = (seq_t)1;
}

/* Ensure dp contains values for indices 0 .. upto (inclusive) */
static void ensure_computed(idx_t upto)
{
    grow_dp(upto);

    for (idx_t i = computed_max + 1; i <= upto; ++i) {
        seq_t sum = (seq_t)0;
        for (int k = 1; k <= 9; ++k) {
            if (i < k) break;
            sum += dp[i - k];
        }
        dp[i] = sum;
    }
    computed_max = upto;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* definition of the sequence */
    ensure_computed(n);
    return dp[n];
}
