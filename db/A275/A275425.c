/*
 * A275425  Number of set partitions of [n] such that seven is a multiple of
 *          each block size.
 * https://oeis.org/A275425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + binomial(n-1,6) * a(n-7) (for n>6) with
 *            a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=1, a(7)=2
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 22, u64 n <= 33, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return binomial(n-1,6) for any n>=7.
   It is calculated as (n-1)(n-2)...(n-6)/720. */
static seq_t binom6(idx_t n_minus1)
{
    seq_t r = (seq_t)1;
    for (idx_t k = 0; k < 6; ++k) {
        r *= (seq_t)(n_minus1 - k);
    }
    /* 6! = 720, division is exact. */
    r /= (seq_t)720;
    return r;
}

seq_t a(idx_t n)
{
    /* Memoisation table --------------------------------------------------- */
    static seq_t *dp    = NULL;   /* array of computed values */
    static idx_t dp_cap = 0;      /* current capacity of dp array */
    static idx_t last   = 0;      /* largest n for which dp[n] is defined */

    /* Handle base cases directly. */
    if (n <= 6) {
        return (seq_t)1;
    }

    /* Initialise the table on the first call. */
    if (dp == NULL) {
        dp_cap = 8;                 /* initial capacity (fits up to n=7) */
        dp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        for (idx_t i = 0; i <= 6; ++i) {
            dp[i] = (seq_t)1;       /* a(0) .. a(6) = 1 */
        }
        dp[7] = (seq_t)2;           /* a(7) = 2 */
        last = 7;
    }

    /* If already present, return it. */
    if (n <= last) {
        return dp[n];
    }

    /* Grow the array if required. */
    if (n >= dp_cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *new_dp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 0; i <= last; ++i) {
            new_dp[i] = dp[i];
        }
        free(dp);
        dp = new_dp;
        dp_cap = new_cap;
    }

    /* Compute values from last+1 up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = binom6(i - 1) * dp[i - 7];
        dp[i] = dp[i - 1] + term;
    }
    last = n;

    return dp[n];
}
