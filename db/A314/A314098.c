/*
 * A314098  Coordination sequence Gal.6.339.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314098
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

/*
 * Formula: a(n) = a(n-1)+a(n-2)-2*a(n-4)+a(n-6)+a(n-7)-a(n-8)   for n>8
 * Initial values: a(0)=1, a(1)=5, a(2)=11, a(3)=16, a(4)=20,
 *                 a(5)=26, a(6)=30, a(7)=34, a(8)=40
 *
 * The function implements the recurrence with memoisation in a static
 * dynamic array.  It works with any seq_t type that is copyable
 * (built‑in integer types or a trivial arbitrary‑precision class).
 */

#include <stdlib.h>

/* Initial values for indices 0..8. */
static const seq_t init_vals[9] = {
    (seq_t)1, (seq_t)5, (seq_t)11, (seq_t)16, (seq_t)20,
    (seq_t)26, (seq_t)30, (seq_t)34, (seq_t)40
};

/* Cached values.  cache[i] holds a(i) for 0 <= i < sz. */
static seq_t *cache   = NULL;   /* pointer to allocated data */
static size_t cap     = 0;      /* allocated capacity */
static size_t sz      = 0;      /* number of values already stored */

/* Allocate new storage and copy existing values. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap)
        return;

    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();   /* out of memory */

    /* copy old values to new storage */
    for (size_t i = 0; i < sz; ++i)
        new_cache[i] = cache[i];

    free(cache);
    cache = new_cache;
    cap   = new_cap;
}

/* Initialise cache with the first nine terms if not already done. */
static void init_cache(void)
{
    ensure_capacity(9);          /* makes sure cache[0..8] exist */
    for (size_t i = 0; i < 9; ++i)
        cache[i] = init_vals[i];
    sz = 9;                      /* indices 0..8 are now valid */
}

/* Public entry point. */
seq_t a(idx_t n)
{
    /* Guard against n being negative (undefined behaviour for unsigned) */
    if (n > SIZE_MAX)   /* not expected in tests */
        abort();

    /* Initialise on first use. */
    if (!cache)
        init_cache();

    /* If the desired value is already cached, return it. */
    if ((size_t)n < sz)
        return cache[n];

    /* Ensure we have space for indices up to n. */
    ensure_capacity((size_t)n + 1);

    /* Compute new values from sz up to n. */
    for (idx_t i = (idx_t)sz; i <= n; ++i)
    {
        if (i < 9) {
            /* The first nine terms were set during init_cache. */
            continue;
        }

        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a4 = cache[i - 4];
        seq_t a6 = cache[i - 6];
        seq_t a7 = cache[i - 7];
        seq_t a8 = cache[i - 8];

        /* a(i) = a(i-1)+a(i-2)-2*a(i-4)+a(i-6)+a(i-7)-a(i-8) */
        seq_t pos = a1 + a2 + a6 + a7;
        seq_t neg = ((seq_t)2 * a4) + a8;
        cache[i] = pos - neg;
    }

    sz = (size_t)n + 1;
    return cache[n];
}
