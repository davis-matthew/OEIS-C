/*
 * A355195  Number of partitions of n that do not contain odd primes as parts.
 * https://oeis.org/A355195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n that do not contain odd primes
 *            as parts * * The function uses a simple dynamic‑programming
 *            (coin‑change) algorithm: * dp[0] = 1 * for each allowed part p
 *            (p not an odd prime) * for i = p .. n * dp[i] += dp[i-p] * * On
 *            the first call or when a larger n is requested...
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   alloc_seq_array, compute_odd_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/compute_odd_primes.h"

/* static state for memoisation */
static seq_t *dp = NULL;            /* table of partition counts */
static size_t dp_cap = 0;           /* current allocated capacity (max_n+1) */

static bool *oddPrime = NULL;       /* flag array for odd primes */
static size_t oddPrime_cap = 0;     /* current capacity of oddPrime */

/* ------------------------------------------------------------------ */
/*   compute_odd_primes:  marks odd primes ≤ n in oddPrime_arr           */

/* ------------------------------------------------------------------ */
/*   compute_dp:  recompute dp and oddPrime for all i ≤ n              */
/* ------------------------------------------------------------------ */
static void compute_dp(idx_t n)
{
    size_t sz = (size_t)n + 1;

    /* allocate new dp array (zero‑initialised) */
    seq_t *new_dp = alloc_seq_array(sz);
    if (!new_dp) return;                /* allocation failure */

    /* free previous tables */
    free(dp);
    dp = new_dp;
    dp_cap = sz;

    /* allocate oddPrime array */
    bool *new_odd = (bool*)calloc(sz, sizeof(bool));
    if (!new_odd)
    {
        free(dp);
        dp = NULL;
        dp_cap = 0;
        return;
    }
    free(oddPrime);
    oddPrime = new_odd;
    oddPrime_cap = sz;

    /* compute odd primes up to n */
    compute_odd_primes(n, oddPrime);

    /* dynamic programming for partitions with allowed parts */
    dp[0] = (seq_t)1;                   /* one way to partition 0 */
    for (idx_t part = 1; part <= n; ++part)
    {
        if (oddPrime[(size_t)part]) continue;   /* skip odd primes */
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }
}

/* ------------------------------------------------------------------ */
/*   a(idx_t n) :  return a(n)   where n ≥ 0                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (dp == NULL || n + 1 > dp_cap)
        compute_dp(n);
    return dp[(size_t)n];
}
