/*
 * A250512  Number of (n+1)X(1+1) 0..3 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A250512
   Recurrence: a(n) = 10*a(n-1) - 15*a(n-2) - 80*a(n-3)
                 + 122*a(n-4) + 228*a(n-5) - 120*a(n-6) - 144*a(n-7)
   Initial values (n >= 1): a(1)=120, a(2)=889, a(3)=5996, a(4)=39187,
                             a(5)=248942, a(6)=1555689, a(7)=9605300
*/

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_alloc_new_array, seq_free */
#include "../utils/seq_free.h"             /* seq_free */

static seq_t *cache = NULL;      /* 1-based array of computed terms */
static idx_t max_computed = 0;   /* highest n computed so far */
static size_t cache_capacity = 0;/* allocated array size (in seq_t units) */

static const seq_t init_vals[8] = {
    0,          /* dummy 0 index */
    120,        /* a(1) */
    889,        /* a(2) */
    5996,       /* a(3) */
    39187,      /* a(4) */
    248942,     /* a(5) */
    1555689,    /* a(6) */
    9605300     /* a(7) */
};

static void ensure_cache(idx_t n)
{
    if (n <= max_computed)
        return;

    /* allocate or grow the cache */
    size_t needed = (size_t)n + 1;   /* 1-based indexing, so +1 */
    if (cache == NULL) {
        cache_capacity = needed;
        cache = seq_alloc_new_array(cache_capacity);
    } else if (needed > cache_capacity) {
        size_t new_cap = needed;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        for (size_t i = 1; i <= (size_t)max_computed; ++i)
            new_cache[i] = cache[i];
        seq_free(cache);
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* compute missing terms */
    for (idx_t idx = max_computed + 1; idx <= n; ++idx) {
        if (idx <= 7) {
            cache[idx] = init_vals[idx];
        } else {
            /* split into positive and negative contributions to avoid
               intermediate signed values */
            seq_t sum_pos = 10 * cache[idx-1] +
                            122 * cache[idx-4] +
                            228 * cache[idx-5];
            seq_t sum_neg = 15 * cache[idx-2] +
                            80 * cache[idx-3] +
                            120 * cache[idx-6] +
                            144 * cache[idx-7];
            cache[idx] = sum_pos - sum_neg;
        }
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    /* n >= 1 for all meaningful calls */
    ensure_cache(n);
    return cache[n];
}
