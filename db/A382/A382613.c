/*
 * A382613  Expansion of 1/(1 - x*(1 + x)^3)^2.
 * https://oeis.org/A382613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A382613
   Formula: a(n) = Sum_{k=0..n} (k+1) * binomial(3*k, n-k)
   Linear recurrence (order 8):
        a(n) = 2*a(n‑1) + 5*a(n‑2) – 13*a(n‑4) – 20*a(n‑5)
             – 15*a(n‑6) – 6*a(n‑7) – a(n‑8)
   Initial values:
        a(0)=1, a(1)=2, a(2)=9, a(3)=28,
        a(4)=88, a(5)=270, a(6)=808, a(7)=2386
   This implementation caches previously computed terms and extends the
   cache using realloc (casted to seq_t* to avoid C++ conversion errors).
   It works for both plain integer builds and for builds where seq_t is an
   arbitrary‑precision integer class.                                    */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;    /* array holding a(0) … a(N) */
static size_t cache_size = 0;  /* number of slots currently filled */
static size_t cache_cap  = 0;  /* current capacity of the array */

/* Ensure the cache has room for indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 256;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* unrecoverable allocation failure */

    cache = tmp;
    cache_cap = new_cap;
}

static const seq_t init_vals[8] = {
    (seq_t)1,  (seq_t)2,  (seq_t)9,  (seq_t)28,
    (seq_t)88, (seq_t)270, (seq_t)808, (seq_t)2386
};

seq_t
a(idx_t n)
{
    /* Return cached value if already computed. */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Make sure the cache can hold the requested index. */
    ensure_cache(n);

    /* Populate the base values if the cache is not yet initialized. */
    if (cache_size < 8) {
        size_t limit = 8;
        if (limit > cache_cap)
            limit = cache_cap;
        for (size_t i = cache_size; i < limit; ++i)
            cache[i] = init_vals[i];
        cache_size = limit;
    }

    /* Compute terms up to the requested index using the recurrence. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i < 8)        /* base case already set */
            continue;

        /* Apply the recurrence: a(i) = 2*a(i-1) + 5*a(i-2) - 13*a(i-4)
           - 20*a(i-5) - 15*a(i-6) - 6*a(i-7) - a(i-8) */
        seq_t val = cache[i - 1];              /* + a(i-1)   */
        val += cache[i - 1];                   /* + a(i-1)   */
        for (int k = 0; k < 5; ++k)            /* +5*a(i-2)  */
            val += cache[i - 2];
        for (int k = 0; k < 13; ++k)           /* -13*a(i-4) */
            val -= cache[i - 4];
        for (int k = 0; k < 20; ++k)           /* -20*a(i-5) */
            val -= cache[i - 5];
        for (int k = 0; k < 15; ++k)           /* -15*a(i-6) */
            val -= cache[i - 6];
        for (int k = 0; k < 6; ++k)            /* -6*a(i-7)  */
            val -= cache[i - 7];
        val -= cache[i - 8];                   /* -a(i-8)    */

        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
