/*
 * A267183  Row 3 of the square array in A267181.
 * https://oeis.org/A267183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) – a(n-4) for n > 4 Initial values:
 *            a(0)=1, a(1)=3, a(2)=4, a(3)=0, a(4)=4
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic array for computed terms */
static size_t cache_size = 0; /* number of elements currently stored */
static size_t cache_cap   = 0; /* allocated capacity */

/* Ensure the cache can hold at least `need` elements */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    /* In case of failure tmp will be NULL; the program can abort safely */
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------------------------------------------------------------------- */
/* Main sequence function                                               */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* integer constants that fit in seq_t are supplied directly */
    const seq_t init[5] = { (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)0, (seq_t)4 };

    /* first call: allocate and initialise the cache with the known
       initial values */
    if (cache_size == 0) {
        size_t need = 5;
        ensure_capacity(need);
        for (size_t i = 0; i < need; ++i) cache[i] = init[i];
        cache_size = need;
    }

    /* handle the trivial case of n < 5 */
    if ((size_t)n < 5) return init[(size_t)n];

    /* compute terms up to the requested index */
    size_t need = (size_t)n + 1;
    ensure_capacity(need);
    for (size_t i = cache_size; i < need; ++i) {
        /* recurrence uses indices i-1, i-3, i-4 which are guaranteed
           already computed because i >= cache_size >= 5 */
        cache[i] = cache[i - 1] + cache[i - 3] - cache[i - 4];
    }
    cache_size = need;

    return cache[(size_t)n];
}
