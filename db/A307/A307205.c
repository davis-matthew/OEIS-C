/*
 * A307205  Coordination sequence for tetravalent node in the first Moore
 *          pentagonal tiling.
 * https://oeis.org/A307205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-8) for n>14; initial values a(0)=1,
 *            a(1)=4, a(2)=8, a(3)=14, a(4)=19, a(5)=24, a(6)=29, a(7)=36,
 *            a(8)=44, a(9)=48, a(10)=54, a(11)=58, a(12)=69, a(13)=68,
 *            a(14)=77
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;     /* allocated array size */
static idx_t  cache_len  = 0;     /* how many entries are filled */

/* pre‑computed initial values a(0) … a(14) */
static const seq_t init_vals[15] = {
    1ULL, 4ULL, 8ULL, 14ULL, 19ULL, 24ULL, 29ULL,
    36ULL, 44ULL, 48ULL, 54ULL, 58ULL, 69ULL, 68ULL, 77ULL
};

static void grow_cache_to(idx_t n)
{
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n < 15) {
        /* n is small – return from the constant table directly */
        return init_vals[(size_t)n];
    }

    /* Ensure the storage can hold up to index n */
    grow_cache_to(n);

    /* Compute any values that have not yet been filled */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 14) {
            cache[i] = init_vals[(size_t)i];
        } else {
            cache[i] = (seq_t)2 * cache[i - 4] - cache[i - 8];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
