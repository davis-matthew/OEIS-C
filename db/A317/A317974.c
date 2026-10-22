/*
 * A317974  a(n) = 2*(a(n-1)+a(n-2)+a(n-3))-a(n-4) for n >= 4, with initial
 *          terms 0,0,1,1.
 * https://oeis.org/A317974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*(a(n-1)+a(n-2)+a(n-3)) - a(n-4)
   for n >= 4,  a(0)=0, a(1)=0, a(2)=1, a(3)=1 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* pointer to cached terms */
static size_t mem_cap = 0;    /* allocated capacity of cache    */
static size_t comp_cap = 0;   /* number of terms that have been computed (next free index) */
static int initialized = 0;  /* whether the base terms have been stored */

/* Expand the cache to hold at least 'need' elements.  Handles both C and C++ builds. */
static void ensure_mem_cap(size_t need)
{
    if (mem_cap >= need) return;

    size_t new_cap = mem_cap ? mem_cap : 1;
    while (new_cap < need) new_cap <<= 1;

#if defined(__cplusplus)
    /* C++: allocate new array and copy existing terms, then delete old array. */
    seq_t *tmp = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < comp_cap; ++i) tmp[i] = cache[i];
        delete [] cache;
    }
    cache = tmp;
#else
    /* C: use realloc (implicitly converts void* to seq_t*). */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { exit(1); }
    cache = tmp;
#endif
    mem_cap = new_cap;
}

/* Public function requested by the build system */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* Initialise the cache with the four base values on the first call. */
    if (!initialized) {
        ensure_mem_cap(4);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        comp_cap = 4;          /* indices 0..3 are now stored */
        initialized = 1;
    }

    /* Make sure we have enough allocated space for the requested index. */
    ensure_mem_cap(idx + 1);

    /* If the term is already computed, return it immediately. */
    if (idx < comp_cap) return cache[idx];

    /* Compute missing terms iteratively up to idx (inclusive). */
    for (size_t i = comp_cap; i <= idx; ++i) {
        /* All indices >= 4 are computed by the recurrence. */
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3];
        cache[i] = 2 * sum - cache[i-4];
    }

    comp_cap = idx + 1;  /* update the number of computed terms */
    return cache[idx];
}
