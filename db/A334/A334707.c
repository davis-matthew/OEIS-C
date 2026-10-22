/*
 * A334707  Number of non-collinear triples in a 5 X n rectangular grid.
 * https://oeis.org/A334707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 599, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A334707: Number of non‑collinear triples in a 5×n grid
   Recurrence (for n > 9):
   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) - a(n-5)
          + a(n-6) - a(n-7) + 2*a(n-8) - a(n-9)
   Initial values:
   a(1)=0, a(2)=100, a(3)=412, a(4)=1056, a(5)=2148,
   a(6)=3820, a(7)=6176, a(8)=9352, a(9)=13456 */
#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for realloc */

static const seq_t init[10] = {
    0,          /* dummy 0 for 0‑based index */
    0,          /* a(1) */
    100,        /* a(2) */
    412,        /* a(3) */
    1056,       /* a(4) */
    2148,       /* a(5) */
    3820,       /* a(6) */
    6176,       /* a(7) */
    9352,       /* a(8) */
    13456       /* a(9) */
};

static seq_t *cache = NULL;        /* cached sequence values */
static size_t cache_cap = 0;        /* capacity of cache (in elements) */
static idx_t cache_upto = 0;        /* highest index currently cached */

/* Grow the cache to at least 'needed' + 1 elements. */
static void ensure_capacity(idx_t needed)
{
    /* needed is 1‑based; we allocate one extra for index 0. */
    if ((size_t)needed + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        if (new_cap <= (size_t)needed + 1) new_cap = needed + 1;
#ifdef __cplusplus
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined, but avoid crash */
        return (seq_t)0;

    if (n <= 9)
        return init[n];

    /* Ensure cache large enough */
    ensure_capacity(n);

    /* Fill initial values if not already present */
    if (cache_upto < 9) {
        idx_t start = cache_upto + 1;
        if (start < 1) start = 1;
        for (idx_t i = start; i <= 9; ++i)
            cache[i] = init[i];
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_upto + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init[i];
            continue;
        }
        seq_t val = 2 * cache[i - 1];
        val -= cache[i - 2];
        val += cache[i - 3];
        val -= cache[i - 4];
        val -= cache[i - 5];
        val += cache[i - 6];
        val -= cache[i - 7];
        val += 2 * cache[i - 8];
        val -= cache[i - 9];
        cache[i] = val;
    }
    cache_upto = n;
    return cache[n];
}
