/*
 * A372803  Expansion of e.g.f. exp(1 - exp(-x)) * (exp(-x) - 1) * (exp(-x) -
 *          2).
 * https://oeis.org/A372803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * Stirling2(n,k) * k^2
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 16, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;    /* cache of sequence values */
static size_t  cache_size = 0;   /* allocated size of a_cache          */
static idx_t   computed_max = 0; /* highest n for which a_cache was filled */

static seq_t *S = NULL;          /* Stirling numbers S[n][k] for current n */
static size_t S_len = 0;         /* length of S (== current n + 1)       */

/* Function to compute a(n) with memoization and incremental S generation */
seq_t a(idx_t n)
{
    /* Initial setup on first call */
    if (a_cache == NULL) {
        a_cache   = (seq_t *)malloc(sizeof(seq_t));
        cache_size = 1;
        a_cache[0] = 0;      /* a(0) = 0 */
        computed_max = 0;

        S = (seq_t *)malloc(sizeof(seq_t));
        S_len = 1;
        S[0] = 1; /* S(0,0) = 1 */
    }

    /* Return cached value if already computed */
    if (n <= computed_max)
        return a_cache[n];

    /* Extend cache and Stirling array up to n, computing a(k) for each new k */
    for (idx_t target = computed_max + 1; target <= n; ++target) {
        /* Ensure S has space for S[target] (indices 0 .. target) */
        if (S_len <= target) {
            S = (seq_t *)realloc(S, (target + 1) * sizeof(seq_t));
            S_len = target + 1;
        }

        /* Update Stirling numbers from n-1 to n (backwards to use previous values) */
        for (idx_t k = target; k >= 1; --k) {
            S[(size_t)k] = (seq_t)k * S[(size_t)k] + S[(size_t)(k - 1)];
        }
        S[0] = 0; /* S(n,0) = 0 for n > 0 */

        /* Compute a(target) = sum_{k=0..target} (-1)^(target-k) * S[k] * k^2 */
        seq_t sum = 0;
        for (idx_t k = 0; k <= target; ++k) {
            seq_t ksq = (seq_t)k * k;                /* k^2 */
            seq_t term = S[(size_t)k] * ksq;         /* S[k] * k^2 */
            if (((target + k) & 1))                 /* apply sign (-1)^(target-k) */
                term = -term;
            sum += term;
        }

        /* Ensure cache has space for index target */
        if (target >= cache_size) {
            a_cache = (seq_t *)realloc(a_cache, (target + 1) * sizeof(seq_t));
            cache_size = target + 1;
        }
        a_cache[target] = sum;
    }

    computed_max = n;
    return a_cache[n];
}
