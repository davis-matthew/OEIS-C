/*
 * A392834  Time taken to get n people from one side of a bridge to the other
 *          where (a) the only flashlight must be carried when crossing; (b)
 *          at most three people may cross at the same time; (c) a group
 *          crosses at the speed of the slowest member; and (d) the k-th
 *          person requires k seconds to cross the bridge.
 * https://oeis.org/A392834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) + 2*n + 6 for n ≥ 13, with initial values a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=7, a(5)=9, a(6)=14, a(7)=17, a(8)=22,
 *            a(9)=26, a(10)=32, a(11)=37, a(12)=43
 * Range:     u8 n <= 33, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init[13] = {
    0ULL, /* unused index 0 */
    1ULL, /* a(1) */
    2ULL, /* a(2) */
    3ULL, /* a(3) */
    7ULL, /* a(4) */
    9ULL, /* a(5) */
   14ULL, /* a(6) */
   17ULL, /* a(7) */
   22ULL, /* a(8) */
   26ULL, /* a(9) */
   32ULL, /* a(10) */
   37ULL, /* a(11) */
   43ULL  /* a(12) */
};

static seq_t *cache = NULL;   /* dynamic memoization array */
static idx_t  cache_size = 0; /* allocated size of cache */

/* External linkage required by the platform */
seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n <= 12) {
        return init[n];
    }

    /* Allocate or enlarge cache if necessary */
    if (cache == NULL || cache_size <= n) {
        idx_t new_size = n + 1;       /* indices 0 … n inclusive */
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* In a real system you might handle allocation failure.
               For the purposes of this task we simply abort. */
            abort();
        }

        /* Copy old data if it exists */
        if (cache != NULL) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }

        /* Initialize the new freed region (optional, zeros anyway) */
        if (new_size > cache_size) {
            /* zero the rest */
            memset(new_cache + cache_size, 0,
                   (new_size - cache_size) * sizeof(seq_t));
        }

        /* Ensure base values are set up to 12 */
        for (idx_t i = 1; i <= 12 && i < new_size; ++i) {
            new_cache[i] = init[i];
        }

        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute values only if they have not been computed yet.
       All values above 12 up to n are now stored in cache[]. */
    for (idx_t i = 13; i <= n; ++i) {
        /* a(i) depends only on a(i-6) which is already computed. */
        cache[i] = cache[i - 6] + (seq_t)(2 * i + 6);
    }

    return cache[n];
}
