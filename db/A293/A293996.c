/*
 * A293996  Number of linear chord diagrams having n chords and no chord
 *          length larger than four.
 * https://oeis.org/A293996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 a(n−1)+2 a(n−2)+11 a(n−3)+60 a(n−4) −18 a(n−5)−26
 *            a(n−6)−108 a(n−7)−48 a(n−8), for n ≥ 8 with initial values
 *            a(0)=1, a(1)=1, a(2)=3, a(3)=15, a(4)=105, a(5)=315, a(6)=1141,
 *            a(7)=4779
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached terms */
static size_t  cache_cap = 0; /* capacity of cache */

seq_t a(idx_t n)
{
    size_t i, cur, sz = (size_t)n;
    if (sz == 0) return 1; /* a(0) */

    /* grow cache to hold terms up to n */
    if (sz + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= sz + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first eight terms once */
    if (cache_cap == 0 || cache[0] == 0) {
        cache[0] = 1;   /* a(0) */
        cache[1] = 1;   /* a(1) */
        cache[2] = 3;   /* a(2) */
        cache[3] = 15;  /* a(3) */
        cache[4] = 105; /* a(4) */
        cache[5] = 315; /* a(5) */
        cache[6] = 1141;/* a(6) */
        cache[7] = 4779;/* a(7) */
    }

    /* compute missing terms up to n using recurrence */
    for (i = cache[0] == 0 ? 8 : 8; i <= sz; ++i) {
        seq_t t = 0;
        t = 2  * cache[i-1] + 2  * cache[i-2]
          + 11 * cache[i-3] + 60 * cache[i-4]
          - 18 * cache[i-5] - 26 * cache[i-6]
          - 108 * cache[i-7] - 48 * cache[i-8];
        cache[i] = t;
    }

    return cache[sz];
}
