/*
 * A375821  Number of ways to tile a 3-row parallelogram of length n with
 *          triangular and rectangular tiles, each of size 3.
 * https://oeis.org/A375821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-3) - a(n-4) - a(n-6) Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=7, a(4)=17, a(5)=41
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    if (N < cache_len) {
        return cache[N];
    }

    if (N >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap;
        while (new_cap <= N) {
            new_cap <<= 1;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    for (size_t i = cache_len; i <= N; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i == 1) {
            cache[i] = 1;
        } else if (i == 2) {
            cache[i] = 2;
        } else if (i == 3) {
            cache[i] = 7;
        } else if (i == 4) {
            cache[i] = 17;
        } else if (i == 5) {
            cache[i] = 41;
        } else {
            seq_t term = mul_small(cache[i-1], 2);          // 2*a(i-1)
            term += mul_small(cache[i-3], 4);              // +4*a(i-3)
            term -= cache[i-4];                            // -a(i-4)
            term -= cache[i-6];                            // -a(i-6)
            cache[i] = term;
        }
    }

    cache_len = N + 1;
    return cache[N];
}
