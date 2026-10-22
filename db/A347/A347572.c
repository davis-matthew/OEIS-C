/*
 * A347572  Number of partitions of n into 5 or more distinct parts.
 * https://oeis.org/A347572
 *
 * Offset:    15    (a(15) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into 5 or more distinct parts
 * Range:     u8 n <= 34, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   p_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/p_index.h"

/* seq_t and idx_t are supplied by the build system.  */
/* No need to declare them; they are already defined.  */

/* Static storage for the table of ordinary partitions P[n][k].  */
/* The table is flattened to a one‑dimensional array.  */
static seq_t *P      = NULL;   /* DP table data */
static idx_t P_nmax  = 0;      /* largest n stored so far */
static idx_t P_kmax  = 0;      /* largest k stored so far */

/* Recompute the DP table so that it contains values for all
   0 ≤ n ≤ N and 0 ≤ k ≤ kmax, where kmax is the largest k such
   that k(k+1)/2 ≤ N (the maximum number of distinct parts that can
   appear in a partition of N).  */
static void
recompute_dp(idx_t N)
{
    /* Determine the maximum number of parts needed for this N. */
    idx_t kmax = 0;
    while ((kmax + 1) * (kmax + 2) / 2 <= N)
        ++kmax;

    /* If the current table is already large enough, do nothing. */
    if (N <= P_nmax && kmax <= P_kmax)
        return;

    /* Allocate a new table.  All elements are initialised to zero. */
    size_t cells   = (size_t)(N + 1) * (kmax + 1);
    seq_t *newP    = (seq_t *)calloc(cells, sizeof(seq_t));
    if (!newP)       /* allocation failure – keep the old table */
        return;

    /* Classic recurrence for number of partitions of n into exactly k
       positive parts:  P[n][k] = P[n-1][k-1] + P[n-k][k]  (n ≥ k).  */
    for (idx_t n = 0; n <= N; ++n) {
        for (idx_t k = 0; k <= n && k <= kmax; ++k) {
            if (k == 0) {
                newP[p_index(n, k, kmax)] = (n == 0 ? (seq_t)1 : (seq_t)0);
            } else {
                seq_t a = (n >= 1) ? newP[p_index(n - 1, k - 1, kmax)] : (seq_t)0;
                seq_t b = (n >= k) ? newP[p_index(n - k,   k,     kmax)] : (seq_t)0;
                newP[p_index(n, k, kmax)] = a + b;
            }
        }
    }

    /* Replace the old table. */
    free(P);
    P       = newP;
    P_nmax  = N;
    P_kmax  = kmax;
}

/* Public function computing a(n) for n ≥ 15.  */
seq_t
a(idx_t n)
{
    if (n < 15)                /* out of defined range */
        return (seq_t)0;

    recompute_dp(n);           /* ensure the DP table covers this n */

    seq_t result = (seq_t)0;
    for (idx_t k = 5; ; ++k) {
        /* Minimum sum of k distinct positive integers = k(k+1)/2. */
        idx_t min_sum = k * (k + 1) / 2;
        if (min_sum > n)
            break;            /* no larger k can contribute */

        /* Map to the ordinary partition problem: reduce the sum by
           subtracting 0,1,...,k-1 from the distinct parts. */
        idx_t m = n - k * (k - 1) / 2;
        if (m >= k)          /* partition into k parts is possible */
            result += P[p_index(m, k, P_kmax)];
    }

    return result;
}
