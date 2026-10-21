/*
 * A245506  Number of endofunctions f on [n] such that f^10(i) = f(i) for all
 *          i in [n].
 * https://oeis.org/A245506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) =  Σ_{C=0..n} binom(n,C) · p(C) · C^{n-C}
 * where p(C) = Σ_{k+3ℓ+9m=C} (C! / (k! ℓ! m! 1^k 3^ℓ 9^m))
 *  = number of permutations of C elements with all cycles of length 1, 3 or 9.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Maximum number of indices we prepare – enough for the tests. */
#define MAX_REQ 10000

/* Static cache for the sequence and the auxiliary p(C) values. */
static seq_t *cache_a = NULL;
static seq_t *cache_p = NULL;
static idx_t   cache_limit = 0;

/* Forward declaration of routine that builds/extends the cache. */
static void build_cache(idx_t limit);

/* ------------------------------------------------------------------ */
/* Compute p(C) for C ≤ limit.  The recurrence follows from the
 * combinatorial definition:
 *
 *   p(0)=1,
 *   p(i)=p(i-1)                           // add a fixed point
 *        +(i-1)(i-2)·p(i-3)               // add a 3-cycle
 *        +(i-1)(i-2)…(i-8)·p(i-9)         // add a 9-cycle
 *
 * The products are computed iteratively to keep intermediate values
 * reasonable.
 */
static void build_cache(idx_t limit)
{
    if (limit > MAX_REQ) limit = MAX_REQ;

    /* Allocate new tables.  calloc zeros them. */
    seq_t *p_new = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    seq_t *a_new = (seq_t *)calloc((size_t)(limit + 1), sizeof(seq_t));
    if (!p_new || !a_new) {        /* allocation failed – bail out */
        free(p_new); free(a_new);
        return;
    }

    /* ---- Compute p(C)  ------------------------------------------- */
    p_new[0] = (seq_t)1;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t val = (seq_t)0;

        /* cycle length 1 */
        val += p_new[i - 1];

        /* cycle length 3 */
        if (i >= 3) {
            seq_t prod = (seq_t)1;
            for (idx_t j = i - 2; j <= i - 1; ++j)
                prod *= (seq_t)j;
            val += prod * p_new[i - 3];
        }

        /* cycle length 9 */
        if (i >= 9) {
            seq_t prod = (seq_t)1;
            for (idx_t j = i - 8; j <= i - 1; ++j)
                prod *= (seq_t)j;
            val += prod * p_new[i - 9];
        }

        p_new[i] = val;
    }

    /* ---- Compute a(n) -------------------------------------------- */
    for (idx_t C = 0; C <= limit; ++C) {
        seq_t binom = (seq_t)1;   /* binom(C,C) = 1 */
        seq_t pow   = (seq_t)1;   /* C^{0} = 1 */

        for (idx_t n = C; n <= limit; ++n) {
            seq_t term = binom * p_new[C];
            term = term * pow;
            a_new[n] += term;

            if (n == limit) break;

            /* Update binom for the next n+1:  binom(n+1,C) = binom(n,C)*(n+1)/(n+1-C) */
            seq_t numerator   = binom * (seq_t)(n + 1);
            seq_t denominator = (seq_t)(n + 1 - C);
            binom = numerator / denominator;

            /* Update power:  C^{(n+1)-C} = C^{(n-C)} * C */
            pow = pow * (seq_t)C;
        }
    }

    /* Replace old cache with the newly built one. */
    free(cache_a);
    free(cache_p);
    cache_a = a_new;
    cache_p = p_new;
    cache_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Public interface required by the runner. */
seq_t a(idx_t n)
{
    /* Ensure the cache covers the requested index. */
    if (n > cache_limit || cache_a == NULL)
        build_cache(n);

    return cache_a[n];
}
