/*
 * A293995  Number of linear chord diagrams having n chords and no chord
 *          length larger than three.
 * https://oeis.org/A293995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=1, a(2)=3, a(3)=15
   For n≥4: a(n) = 2*a(n-1) + a(n-2) + 8*a(n-3) – 6*a(n-4)
   (derived from the rational generating function
   (1–x)/((3x–1)(2x³–2x²–x–1)) ).
*/
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation array   */
static size_t cache_len = 0;         /* number of filled entries */

seq_t a(idx_t n) {
    /* initialise base cases if not already done */
    if (cache_len == 0) {
        size_t init = 4; /* we need indices 0..3 */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) { /* allocation failed – fallback to simple calculation */
            cache_len = 1;
            cache[0] = (seq_t)1;
            return cache[0];
        }
        cache_len = init;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)15;
    }

    /* extend cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t needed = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return cache[(size_t)n]; /* allocation error – keep old cache */
        cache = tmp;
        for (size_t i = cache_len; i < needed; ++i) {
            if (i < 4) continue;          /* base values already set */
            seq_t val = (seq_t)2 * cache[i-1];
            val += cache[i-2];
            val += (seq_t)8 * cache[i-3];
            val -= (seq_t)6 * cache[i-4];
            cache[i] = val;
        }
        cache_len = needed;
    }

    return cache[(size_t)n];
}
