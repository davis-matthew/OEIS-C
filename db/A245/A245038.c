/*
 * A245038  Number of partitions of n where the minimal multiplicity of any
 *          part is 7.
 * https://oeis.org/A245038
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 174, u32 n <= 449, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A245038: Number of partitions of n with minimal part multiplicity 7.
   Formula: a(n) = p_7(n) – p_8(n), where p_k(n) = number of partitions of n
   in which every part appears at least k times.  The implementation uses a
   direct dynamic‑programming recurrence that runs in O(n²) time and O(n)
   memory, which is fast enough for n up to about 10 000.  The algorithm
   works for any of the supported seq_t types because it relies only on
   basic arithmetic operations. */
#include <stdlib.h>
#include <stdbool.h>

#define MAX_N 10000  /* maximum n expected on the test harness */

/* forward declaration */
static seq_t compute_pk(idx_t n, idx_t k);

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 7)
        return (seq_t)0;                 /* behavior below offset is irrelevant */
    return compute_pk(n, 7u) - compute_pk(n, 8u);
}

/* ------------------------------------------------------------------ */
/* Helper: partitions with minimum multiplicity set to k              */
static seq_t compute_pk(idx_t n, idx_t k)
{
    static seq_t dp[2][MAX_N + 1];
    static seq_t pref[MAX_N + 1];

    /* clear old layer */
    for (idx_t i = 0; i <= n; ++i)
        dp[0][i] = 0;
    dp[0][0] = (seq_t)1;                 /* empty partition */

    /* iterate over part sizes s = 1 … n */
    for (idx_t s = 1; s <= n; ++s) {
        int cur  = (int)(s & 1u);
        int prev = cur ^ 1;

        /* for each residue class modulo s */
        for (idx_t r = 0; r < s; ++r) {
            idx_t maxj = (n - r) / s;     /* largest j such that r + j*s ≤ n */

            /* prefix sums of dp[prev][r + j*s] */
            pref[0] = dp[prev][r];
            for (idx_t j = 1; j <= maxj; ++j)
                pref[j] = pref[j - 1] + dp[prev][r + j * s];

            /* update dp[cur][r + j*s] */
            for (idx_t j = 0; j <= maxj; ++j) {
                seq_t val = dp[prev][r + j * s];
                if (j >= k)
                    val += pref[j - k];   /* add at least k copies of part s */
                dp[cur][r + j * s] = val;
            }
        }
    }

    return dp[n & 1u][n];
}
