/*
 * A337000  E.g.f.: 1 / ((1 - x)*(2 - exp(x))).
 * https://oeis.org/A337000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A337000
 *  Recurrence:
 *      a(0) = 1,  a(1) = 2
 *      a(n) = 2*n*a(n-1) -
 *             Σ_{k=2..n} binomial(n,k)*(k-1)*a(n-k)
 *
 *  The implementation memoises the sequence in a dynamically
 *  resized array.  All arithmetic is performed with the native
 *  type `seq_t`; no floating point or external libraries are used.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"

/* --- Cached values ------------------------------------------- */

static seq_t *cache     = NULL;   /* array of computed terms       */
static size_t cache_cap = 0;     /* allocated capacity, in slots  */
static idx_t  max_computed = 0;  /* largest n already computed    */

/* ---- Ensure that the cache can hold indices up to `n` (inclusive). ---- */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;

    grow_seq(&cache, &cache_cap, new_cap);
}

/* ---- Initialise the first two entries (a(0)=1, a(1)=2). ---- */
static void init_cache(void)
{
    if (max_computed >= 1)
        return;

    ensure_cache(1);
    cache[0] = (seq_t)1;
    if (cache_cap > 1)
        cache[1] = (seq_t)2;
    max_computed = 1;
}

/* ---- Sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    init_cache();                           /* guarantees a(0),a(1) */

    if (n <= max_computed)                  /* already known */
        return cache[n];

    ensure_cache(n);                        /* make room for new values */

    for (idx_t i = max_computed + 1; i <= n; ++i) {

        /* term = 2 * i * a(i-1) */
        seq_t term = ((seq_t)2) * (((seq_t)i) * cache[i - 1]);

        /* S = Σ_{k=2..i} C(i,k)*(k-1)*a(i-k) */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;             /* C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial coefficient to C(i,k) */
            binom = binom * (((seq_t)i) - (seq_t)k + 1) / (seq_t)k;

            if (k < 2)          /* skip k=0,1 */
                continue;

            seq_t tmp = binom * ((seq_t)k - 1);
            tmp      = tmp * cache[i - k];
            sum      += tmp;
        }

        cache[i] = term - sum;                /* a(i) = term – S            */
    }

    max_computed = n;
    return cache[n];
}
