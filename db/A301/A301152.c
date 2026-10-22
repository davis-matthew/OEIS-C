/*
 * A301152  Coordination sequence T3 for Zeolite Code OWE.
 * https://oeis.org/A301152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4)
   + 3*a(n-5) - 2*a(n-6) + 4*a(n-7) - 3*a(n-8) + 4*a(n-9)
   - 4*a(n-10) + 3*a(n-11) - 4*a(n-12) + 2*a(n-13)
   - 3*a(n-14) + 2*a(n-15) - 2*a(n-16) + 2*a(n-17)
   - a(n-18) + a(n-19)   (for n > 19)
   Initial values: a(0)=1, a(1)=4, a(2)=11, a(3)=22, a(4)=32,
   a(5)=49, a(6)=77, a(7)=109, a(8)=139, a(9)=163,
   a(10)=197, a(11)=250, a(12)=310, a(13)=357,
   a(14)=398, a(15)=456, a(16)=526, a(17)=603,
   a(18)=676, a(19)=744 */

#include <stdlib.h>

static const seq_t initial[20] = {
    1,4,11,22,32,49,77,109,139,163,197,250,310,357,398,456,526,603,676,744
};

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements currently stored */

static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need < 20) need = 20;

    if (!cache) {                                 /* first allocation */
        cache = (seq_t*)malloc(need * sizeof(seq_t));
        if (!cache) exit(1);
        for (size_t i = 0; i < 20; ++i) cache[i] = initial[i];
        cache_len = 20;
    } else if (need > cache_len) {                /* enlarge existing cache */
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        /* preserve initial values if they were not yet stored */
        if (cache_len < 20) {
            for (size_t i = cache_len; i < 20; ++i) cache[i] = initial[i];
        }
        /* cache_len stays unchanged, the new space is empty */
    }

    size_t start = cache_len;                      /* compute missing terms */
    for (size_t k = start; k <= (size_t)n; ++k) {
        if (k < 20) {
            cache[k] = initial[k];
        } else {
            cache[k] =  cache[k-1] - 2*cache[k-2] + 2*cache[k-3] - 2*cache[k-4]
                       + 3*cache[k-5] - 2*cache[k-6] + 4*cache[k-7] - 3*cache[k-8]
                       + 4*cache[k-9] - 4*cache[k-10] + 3*cache[k-11] - 4*cache[k-12]
                       + 2*cache[k-13] - 3*cache[k-14] + 2*cache[k-15] - 2*cache[k-16]
                       + 2*cache[k-17] - cache[k-18] + cache[k-19];
        }
    }
    cache_len = (size_t)n + 1;                     /* update stored count */
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
