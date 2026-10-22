/*
 * A375867  E.g.f. satisfies A(x) = exp( 2 * (exp(x*A(x)^(1/2)) - 1) ).
 * https://oeis.org/A375867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k=0..n} (n+2)^(k-1) * Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed from the explicit formula, using Stirling numbers of the
 * second kind. The sequence is memoised for arbitrary‑order calls. */

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

static seq_t *Stirling = NULL;      /* row by row, 0 <= k <= current n */
static seq_t *cache    = NULL;      /* memoised a(n) values */
static idx_t  cache_cap = 0;        /* current capacity of Stirling/cache */
static idx_t  last_n    = 0;        /* highest n that has been computed */

/* Ensure Stirling and cache are large enough for indices 0..n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;

    idx_t new_cap = n + 1;
    seq_t *newS  = seq_alloc_new_array(new_cap);
    seq_t *newC  = seq_alloc_new_array(new_cap);

    /* Initialise to zero to avoid use of garbage */
    for (idx_t i = 0; i < new_cap; ++i) {
        newS[i] = 0;
        newC[i] = 0;
    }

    /* Copy existing data, if any, into the new arrays */
    if (Stirling) {
        for (idx_t i = 0; i < cache_cap; ++i)
            newS[i] = Stirling[i];
        free_seq_array(Stirling);
    }

    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i)
            newC[i] = cache[i];
        free_seq_array(cache);
    }

    Stirling = newS;
    cache    = newC;
    cache_cap = new_cap;
}

/* Initialise arrays on the first call */
static void init_if_needed(void)
{
    if (Stirling == NULL) {
        Stirling = seq_alloc_new_array(1);
        Stirling[0] = 1;            /* S(0,0) = 1 */
    }
    if (cache == NULL) {
        cache = seq_alloc_new_array(1);
        cache[0] = 1;                /* a(0) = 1 */
        cache_cap = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* a(0) = 1 defined explicitly */

    init_if_needed();
    ensure_capacity(n);

    /* Compute from last_n + 1 up to n */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        /* Prepare Stirling[0] for the new row */
        if (i == 1)
            Stirling[0] = 1;          /* S(0,0) from previous row */
        else
            Stirling[0] = 0;          /* S(n-1,0) = 0 for n>1 */

        /* Update Stirling numbers for row i using the recurrence:
         * S(i,k) = k * S(i-1,k) + S(i-1,k-1)   (backward loop)
         */
        for (idx_t k = i; k >= 1; --k) {
            Stirling[k] = (k * Stirling[k]) + Stirling[k - 1];
        }

        /* Weighted sum: sum_{k=1..i} (i+2)^(k-1) * S(i,k) */
        seq_t sum   = 0;
        seq_t power = 1;                 /* (i+2)^(k-1) for current k */
        seq_t base  = (seq_t)(i + 2);

        for (idx_t k = 1; k <= i; ++k) {
            sum   = sum + power * Stirling[k];
            power = power * base;
        }

        cache[i] = sum * 2;   /* final factor of 2 */
    }

    last_n = n;
    return cache[n];
}
