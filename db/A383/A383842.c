/*
 * A383842  Expansion of 1/((1-x) * (1-2*x) * (1-3*x) * (1-4*x))^2.
 * https://oeis.org/A383842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} Stirling2(k+4,4) * Stirling2(n-k+4,4)
 *            recurrence: a(n) = 20*a(n-1) - 170*a(n-2) + 800*a(n-3) -
 *            2273*a(n-4) + 3980*a(n-5) - 4180*a(n-6) + 2400*a(n-7) -
 *            576*a(n-8) initial values: a(0)=1, a(1)=20, a(2)=230, a(3)=2000,
 *            a(4)=14627, a(5)=95060, a(6)=567240, a(7)=3174400 The impleme...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[8] = {
    (seq_t)1,
    (seq_t)20,
    (seq_t)230,
    (seq_t)2000,
    (seq_t)14627,
    (seq_t)95060,
    (seq_t)567240,
    (seq_t)3174400
};

seq_t a(idx_t n)
{
    /* static cache for all computed values */
    static seq_t *cache = NULL;
    static size_t cache_used = 0;   /* number of initialized entries */
    static size_t cache_capacity = 0; /* allocated capacity */

    if (cache_used <= (size_t)n) {
        /* extend capacity if needed */
        size_t need = (size_t)n;
        if (need >= cache_capacity) {
            size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
            if (new_cap <= need + 1) new_cap = need + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_capacity = new_cap;
        }
        /* compute terms up to n */
        for (size_t i = cache_used; i <= need; ++i) {
            if (i < 8) {
                cache[i] = init[i];
            } else {
                seq_t val = (seq_t)20 * cache[i-1];
                val -= (seq_t)170 * cache[i-2];
                val += (seq_t)800 * cache[i-3];
                val -= (seq_t)2273 * cache[i-4];
                val += (seq_t)3980 * cache[i-5];
                val -= (seq_t)4180 * cache[i-6];
                val += (seq_t)2400 * cache[i-7];
                val -= (seq_t)576 * cache[i-8];
                cache[i] = val;
            }
        }
        cache_used = need + 1;
    }
    return cache[(size_t)n];
}
