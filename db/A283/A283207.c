/*
 * A283207  a(n) = a(floor(n/a(n-1))) + a(floor(n/a(n-2))) with a(1) = a(2) =
 *          2.
 * https://oeis.org/A283207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A283207
 *  Recurrence:
 *  a(1) = a(2) = 2
 *  a(n) = a(floor(n / a(n-1))) + a(floor(n / a(n-2)))   for n ≥ 3
 *  (indices are 1‑based; a(0) is never used)
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for computed values */
static seq_t *a_cache    = NULL;   /* array of size cap, index 0 unused       */
static idx_t   a_cap     = 0;      /* current allocated capacity (number of
                                     slots)                                     */
static idx_t   a_computed = 0;     /* largest index that has a valid value */

/* ensure that the cache has at least n+1 slots and that the
   base values a(1) and a(2) are defined; after this call
   a_computed is still the index of the last computed term. */
static void
a_ensure(idx_t n)
{
    /* Grow storage if necessary */
    if (a_cap < n + 1) {
        seq_t *tmp = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (!tmp)
            abort();                       /* allocation failure */
        a_cache = tmp;
        a_cap   = n + 1;
    }

    /* Initialise base values if not yet done */
    if (a_computed < 2) {
        a_cache[1] = (seq_t)2;
        a_cache[2] = (seq_t)2;
        if (a_computed < 1) a_computed = 1;
        if (a_computed < 2) a_computed = 2;
    }
}

/* compute all a(k) for k = a_computed+1 … n */
static void
a_compute_to(idx_t n)
{
    if (a_computed >= n) return;          /* nothing to do */

    for (idx_t k = a_computed + 1; k <= n; ++k) {
        /* floor(k / a(k-1)) and floor(k / a(k-2)) are at least 1
           because a(k-1), a(k-2) ≤ 2k (power of two bound). */
        idx_t i1 = k / (idx_t)a_cache[k - 1];
        idx_t i2 = k / (idx_t)a_cache[k - 2];
        a_cache[k] = a_cache[i1] + a_cache[i2];
    }
    a_computed = n;
}

/* public routine implementing the sequence */
seq_t
a(idx_t n)
{
    if (n <= 2)
        return (seq_t)2;

    /* ensure storage and base values are ready */
    a_ensure(n);

    /* compute missing terms up to n */
    a_compute_to(n);

    return a_cache[n];
}
