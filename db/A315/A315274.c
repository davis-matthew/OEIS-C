/*
 * A315274  Coordination sequence Gal.6.354.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 6. For n ≥ 2: if n is even, let k = n/2. a(n) =
 *            a(n−2) + (k==1 ? 9 : (k−2)%3==0 ? 11 : 10) // pattern
 *            9,11,10,10… if n is odd, let j = (n−3)/2. a(n) = a(n−2) +
 *            {9,11,10,11,9,12}[ j%6 ] // periodic increment
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoization array */
static idx_t  computed = 0;      /* number of entries in cache */
static idx_t  capacity = 0;      /* current allocated capacity */

/* Ensure the cache can hold indices up to n.  */
static void grow_cache_to(idx_t n)
{
    if (n < capacity) return;
    /* Allocate a bit more space than needed to reduce reallocs. */
    idx_t new_cap = (n + 1) * 2;
    if (new_cap < 32) new_cap = 32;            /* minimal reasonable size */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();    /* allocation failure */
    cache   = tmp;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if (n < computed) return cache[n];

    /* Make sure the array has enough capacity. */
    grow_cache_to(n);

    /* Initialise base cases if they are not yet present. */
    if (computed == 0) {
        cache[0] = 1;
        computed = 1;
    }
    if (computed <= 1 && n >= 1) {
        cache[1] = 6;
        computed = 2;
    }

    /* Incrementally compute values up to n. */
    static const seq_t even_inc[3] = {11, 10, 10};
    static const seq_t odd_inc [6] = {9, 11, 10, 11, 9, 12};

    for (idx_t i = computed; i <= n; ++i) {
        if ((i & 1ULL) == 0) {          /* even i */
            idx_t k = i >> 1;           /* k = i/2 */
            seq_t inc = (k == 1) ? 9
                                 : even_inc[(k - 2) % 3];
            cache[i] = cache[i-2] + inc;
        } else {                        /* odd i */
            /* ii = (i-3)/2 is used to index the 6‑cycle. */
            idx_t ii = (i - 3) >> 1;
            seq_t inc = odd_inc[ii % 6];
            cache[i] = cache[i-2] + inc;
        }
    }

    computed = n + 1;   /* we have stored values 0 .. n */
    return cache[n];
}
