/*
 * A332874  Number of strict compositions of n that are neither unimodal nor
 *          is their negation.
 * https://oeis.org/A332874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥4} (k! – 2^k + 2) * c(k,n) where c(k,n) = number of
 *            subsets of k distinct positive integers summing to n
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 74, u64 n <= 189,
 *            u128 n <= 513, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number of subsets of size k with sum n equals the number of partitions of
   n – k(k+1)/2 into at most k parts, which can be computed by the
   recurrence:
        p[0][0] = 1;   p[0][t>0] = 0
        p[k][t] = p[k‑1][t] + (t≥k ? p[k][t‑k] : 0)              (t ≥ 0).
   The table p[k][t] counts partitions of t with parts ≤ k (equivalently
   partitions into at most k parts). */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *dp      = NULL;   /* 1‑D array of size (kmax+1)*(nmax+1) */
static seq_t *fact    = NULL;
static seq_t *pow2    = NULL;
static idx_t  dp_nmax = 0;      /* current maximum n that dp covers   */
static size_t dp_kmax = 0;      /* current maximum k that dp covers   */

/* Ensure that dp, fact and pow2 contain data up to at least 'n'. */
static void ensure_tables(idx_t n)
{
    /* Determine the largest k needed for this n:
         k * (k+1) / 2 ≤ n .
       Since n ≤ 10000, k ≤ 140, a simple loop is fast enough. */
    size_t kmax_needed = 0;
    while (((idx_t)kmax_needed * (kmax_needed + 1)) / 2 <= n) ++kmax_needed;

    if (dp_nmax >= n && dp_kmax >= kmax_needed)
        return;   /* already up to date */

    /* Deallocate old tables if present */
    if (dp)      free(dp);
    if (fact)    free(fact);
    if (pow2)    free(pow2);

    dp_nmax  = n;
    dp_kmax  = kmax_needed;

    /* Allocate new tables */
    dp    = (seq_t *)calloc((dp_kmax + 1) * (dp_nmax + 1), sizeof(seq_t));
    fact  = (seq_t *)calloc(dp_kmax + 1, sizeof(seq_t));
    pow2  = (seq_t *)calloc(dp_kmax + 1, sizeof(seq_t));

    if (!dp || !fact || !pow2) exit(1);   /* allocation failure */

    /* Initialize p[0][*] */
    for (idx_t t = 0; t <= dp_nmax; ++t)
        dp[t] = (t == 0 ? (seq_t)1 : (seq_t)0);

    /* Compute factorials and powers of two */
    fact[0] = (seq_t)1;
    pow2[0] = (seq_t)1;
    for (size_t i = 1; i <= dp_kmax; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
        pow2[i] = pow2[i - 1] * (seq_t)2;
    }

    /* Compute the DP table for partitions into at most k parts */
    idx_t stride = dp_nmax + 1;
    for (size_t k = 1; k <= dp_kmax; ++k) {
        for (idx_t t = 0; t <= dp_nmax; ++t) {
            seq_t val = dp[(k - 1) * stride + t];   /* part 0 */
            if (t >= k)
                val += dp[k * stride + (t - k)];    /* take a part of size k */
            dp[k * stride + t] = val;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* a(0) = 0 */
    ensure_tables(n);

    size_t stride = dp_nmax + 1;
    seq_t result  = (seq_t)0;

    for (size_t k = 4; k <= dp_kmax; ++k) {
        idx_t min_sum = ((idx_t)k * (k + 1)) / 2;
        if (min_sum > n) break;          /* further k will only increase min_sum */

        idx_t s = n - min_sum;           /* s = n - k(k+1)/2 */
        seq_t weight = fact[k] - pow2[k] + (seq_t)2;
        seq_t cnt = dp[k * stride + s];
        result += weight * cnt;
    }
    return result;
}
