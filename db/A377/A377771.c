/*
 * A377771  Number of edge cuts in the n-trapezohedral graph.
 * https://oeis.org/A377771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A377771:
   a(n) = 39*a(n-1) - 531*a(n-2) + 2997*a(n-3) - 6588*a(n-4)
          + 5956*a(n-5) - 2128*a(n-6) + 256*a(n-7)
   with a(0)=1, a(1)=13, a(2)=185, a(3)=3013, a(4)=51009,
   a(5)=864453, a(6)=14514449 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t base_vals[7] = {
    (seq_t)1,
    (seq_t)13,
    (seq_t)185,
    (seq_t)3013,
    (seq_t)51009,
    (seq_t)864453,
    (seq_t)14514449
};

static seq_t *cache = NULL;       /* pointer to stored values */
static size_t cache_len = 0;      /* first index not yet computed */
static size_t cache_cap = 0;      /* allocated capacity */

/* Allocate an array of seq_t of given capacity */
static seq_t *alloc_array(size_t cap) {
#if defined(__cplusplus)
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

/* Resize the cache to at least new_cap elements */
static void ensure_capacity(size_t new_cap) {
    if (new_cap <= cache_cap)
        return;

    /* Allocate a new buffer */
    seq_t *new_buf = alloc_array(new_cap);

    /* Copy existing contents (if any) */
    for (size_t i = 0; i < cache_len; ++i)
        new_buf[i] = cache[i];

    /* Free old buffer */
#if defined(__cplusplus)
    delete[] cache;
#else
    free(cache);
#endif

    cache = new_buf;
    cache_cap = new_cap;
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    /* Ensure enough storage for index n */
    ensure_capacity((size_t)(n + 1));

    /* Compute terms up to n if needed */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i < 7) {
            cache[i] = base_vals[(size_t)i];
        } else {
            seq_t t1 = ((seq_t)39)  * cache[(size_t)i-1];
            seq_t t2 = ((seq_t)531) * cache[(size_t)i-2];
            seq_t t3 = ((seq_t)2997) * cache[(size_t)i-3];
            seq_t t4 = ((seq_t)6588) * cache[(size_t)i-4];
            seq_t t5 = ((seq_t)5956) * cache[(size_t)i-5];
            seq_t t6 = ((seq_t)2128) * cache[(size_t)i-6];
            seq_t t7 = ((seq_t)256)  * cache[(size_t)i-7];

            seq_t pos = t1 + t3 + t5 + t7;
            seq_t neg = t2 + t4 + t6;
            cache[(size_t)i] = pos - neg;
        }
    }

    cache_len = (size_t)(n + 1);
    return cache[(size_t)n];
}
