/*
 * A315633  Coordination sequence Gal.6.261.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A315633: Coordination sequence Gal.6.261.6
   Recurrence: a(n) = a(n-1) + a(n-11) - a(n-12)  for n > 12
   Initial values:
     a(0)=1, a(1)=6, a(2)=12, a(3)=16, a(4)=20, a(5)=24, a(6)=30,
     a(7)=34, a(8)=38, a(9)=42, a(10)=48, a(11)=54, a(12)=60
   (All indices are 0‑based.) */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;        /* array of sequence values */
static size_t cap = 0;             /* capacity of the cache */
static idx_t max_computed = (idx_t)-1;  /* highest index computed so far */

/* Ensure the cache holds indices 0 … n inclusive.  If necessary, grow it. */
static void ensure_capacity(idx_t n)
{
    if (n >= (idx_t)cap)
    {
        size_t new_cap = (size_t)n + 1;
        /* double the capacity if possible to reduce reallocations */
        if (cap > 0 && new_cap < cap * 2)
            new_cap = cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) /* allocation failed – abort */
            abort();
        cache = new_cache;
        cap = new_cap;
    }
}

/* Initialise the cache with the first 13 terms. */
static void init_cache(void)
{
    ensure_capacity(12);          /* capacity at least 13 */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)6;
    cache[2] = (seq_t)12;
    cache[3] = (seq_t)16;
    cache[4] = (seq_t)20;
    cache[5] = (seq_t)24;
    cache[6] = (seq_t)30;
    cache[7] = (seq_t)34;
    cache[8] = (seq_t)38;
    cache[9] = (seq_t)42;
    cache[10] = (seq_t)48;
    cache[11] = (seq_t)54;
    cache[12] = (seq_t)60;
    max_computed = 12;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (cache == NULL)
        init_cache();

    if (n <= max_computed)
        return cache[(size_t)n];

    /* Compute values up to n, extending the cache if needed */
    if (n > max_computed)
    {
        ensure_capacity(n);          /* guarantee storage for n */
        for (idx_t i = max_computed + 1; i <= n; ++i)
        {
            seq_t term = cache[(size_t)(i - 1)];
            term = term + cache[(size_t)(i - 11)] - cache[(size_t)(i - 12)];
            cache[(size_t)i] = term;
        }
        max_computed = n;
    }

    return cache[(size_t)n];
}
