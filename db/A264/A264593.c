/*
 * A264593  Let G[1](q) denote the g.f. for A003114 and G[2](q) the g.f. for
 *          A003106 (the two Rogers-Ramanujan identities). For i>=3, let
 *          G[i](q) = (G[i-1](q)-G[i-2](q))/q^(i-2). Sequence gives
 *          coefficients of G[6](q).
 * https://oeis.org/A264593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∑_{m≥0} q^{m*(m+5)} ÷ ∏_{j=1}^m
 *            (1-q^j) Computation: dynamic programming that builds the
 *            standard partition generating function for parts ≤ m
 *            sequentially and adds its shifted contributions to the result
 *            array
 * Range:     u8 n <= 56, u16 n <= 129, u32 n <= 350, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed a(n) values          */
static idx_t  cache_size = 0; /* number of values stored in cache       */

/* Recompute the sequence up to and including index N. */
static void recompute(idx_t N)
{
    /* allocate temporary arrays */
    seq_t *a = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *dp = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!a || !dp) return;            /* allocation failure - abort */

    /* initial partition counts with no part sizes allowed */
    dp[0] = (seq_t)1;                  /* only the empty partition */

    /* determine maximum m such that m*(m+5) <= N */
    idx_t m = 0;
    while ((m + 1) * ((m + 1) + 5) <= N) ++m;

    /* iterate over all m from 0 to M */
    for (idx_t mm = 0; mm <= m; ++mm) {
        if (mm > 0) {
            /* add unlimited parts of size mm */
            for (idx_t t = mm; t <= N; ++t) {
                dp[t] += dp[t - mm];
            }
        }

        /* offset for the current m */
        seq_t offset = (seq_t)mm * ((seq_t)mm + 5);
        if (offset > (seq_t)N) break;  /* no further contributions */

        /* accumulate contributions to a[n] */
        for (idx_t n = (idx_t)offset; n <= N; ++n) {
            a[n] += dp[n - (idx_t)offset];
        }
    }

    /* replace the old cache by the new one */
    free(cache);
    cache      = a;
    cache_size = N + 1;
    free(dp);
}

/* Public entry point – returns a(n). */
seq_t a(idx_t n)
{
    if (cache == NULL || n >= cache_size) {
        recompute(n);
    }
    return cache[n];
}
