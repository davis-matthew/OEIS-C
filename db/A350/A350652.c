/*
 * A350652  a(n) is where the chosen card needs to be placed in a deck of 2n
 *          cards when performing "Persistimis Possessiamo" trick.
 * https://oeis.org/A350652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (n + 1 - a(floor(n/2))) with a(1)=2
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    if (n == 0) return 0;
    if (cache_cap < 1) {
        cache = (seq_t*)realloc(cache, sizeof(seq_t));
        if (!cache) return 0;
        cache[0] = (seq_t)2;
        cache_cap = 1;
    }
    if (n > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            idx_t half = i / 2;
            seq_t val = (seq_t)2 * (((seq_t)i) + (seq_t)1 - cache[half - 1]);
            cache[i - 1] = val;
        }
        cache_cap = n;
    }
    return cache[n - 1];
}
