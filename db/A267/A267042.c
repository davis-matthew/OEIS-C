/*
 * A267042  Decimal representation of the n-th iteration of the "Rule 91"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-2)-84*a(n-4)+64*a(n-6) for n > 8 with initial
 *            values a(0)=1, a(1)=5, a(2)=10, a(3)=99, a(4)=124, a(5)=1935,
 *            a(6)=496, a(7)=32319, a(8)=1984
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t* cache = NULL;            /* cached values  */
static size_t cache_cap = 0;           /* size of the cache array    */
static idx_t last_computed = 0;        /* largest n for which a(n) is cached */

static const seq_t base_values[9] = {
    1,          /* a(0) */
    5,          /* a(1) */
    10,         /* a(2) */
    99,         /* a(3) */
    124,        /* a(4) */
    1935,       /* a(5) */
    496,        /* a(6) */
    32319,      /* a(7) */
    1984        /* a(8) */
};

static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n + 1) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t* new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) return;             /* allocation failed – let the caller use 0 */
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache and base values on first call */
    if (!cache) {
        ensure_cache(8);
        for (idx_t i = 0; i < 9; ++i) cache[i] = base_values[i];
        last_computed = 8;
    }
    if (n < 9) return cache[n];

    /* extend cache if needed */
    if ((idx_t)cache_cap <= n) ensure_cache(n);

    /* compute new values only for the missing indices */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t t1, t2, t3;
#if defined(__SIZEOF_INT128__)  /* use 128‑bit intermediate when possible */
        if (sizeof(seq_t) <= 8) {
            t1 = (seq_t)((unsigned __int128)cache[i-2] * 21ULL);
            t2 = (seq_t)((unsigned __int128)cache[i-4] * 84ULL);
            t3 = (seq_t)((unsigned __int128)cache[i-6] * 64ULL);
        } else
#endif
        {
            t1 = 21 * cache[i-2];
            t2 = 84 * cache[i-4];
            t3 = 64 * cache[i-6];
        }
        cache[i] = t1 + t3 - t2;       /* a(i) = 21*a(i-2) + 64*a(i-6) - 84*a(i-4) */
    }
    last_computed = n;
    return cache[n];
}
