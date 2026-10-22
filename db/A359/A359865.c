/*
 * A359865  a(n) is the number of k > 0 such that n-1-2*k >= 0 and a(n-1-2*k)
 *          * a(n-1) = a(n-1-k)^2.
 * https://oeis.org/A359865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n<3; for n>=3, a(n) = #{ k>0 | n-1-2k>=0 and
 *            a(n-1-2k)*a(n-1) == a(n-1-k)^2 }
 * Range:     u8 n <= 855, u16 n <= 6243, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* abort */
#include <stdbool.h>         /* bool */
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static seq_t *cache   = NULL;   /* cache of computed terms */
static idx_t  capacity = 0;     /* allocated capacity of cache */
static idx_t  computed = 0;     /* number of terms currently stored */

/* public sequence function */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold up to term n */
    if (n + 1 > capacity) {
        idx_t new_cap = capacity ? capacity : 1;
        while (n + 1 > new_cap) new_cap <<= 1;
        seq_t *new_cache = alloc_seq(new_cap);
        if (!new_cache) abort(); /* allocation failure */

        /* copy existing values */
        if (cache) {
            for (idx_t i = 0; i < computed; ++i)
                new_cache[i] = cache[i];
            free_seq(cache);
        }
        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute missing terms up to n (in order). */
    if (computed <= n) {
        for (idx_t i = computed; i <= n; ++i) {
            if (i < 3) {
                cache[i] = 0;
            } else {
                seq_t count = 0;
                idx_t limit = (i - 1) / 2;
                seq_t prev = cache[i - 1];
                for (idx_t k = 1; k <= limit; ++k) {
                    idx_t idxA = i - 1 - 2 * k;
                    idx_t idxB = i - 1 - k;
                    seq_t left  = cache[idxA] * prev;
                    seq_t right = cache[idxB] * cache[idxB];
                    if (left == right) ++count;
                }
                cache[i] = count;
            }
        }
        computed = n + 1;
    }

    return cache[n];
}
