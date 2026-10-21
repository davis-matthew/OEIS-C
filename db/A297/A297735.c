/*
 * A297735  Number of 3Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 or 4 neighboring 1s.
 * https://oeis.org/A297735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +4*a(n-2) -6*a(n-3) +7*a(n-4) -12*a(n-5)
 *            -19*a(n-6) +16*a(n-7) +10*a(n-8) +4*a(n-9) Initial values:
 *            a(1)=1, a(2)=3, a(3)=17, a(4)=34, a(5)=92, a(6)=223, a(7)=700,
 *            a(8)=1747, a(9)=4931
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_max = 0;

static void grow_cache(idx_t need) {
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (cache_max == 0) {
        grow_cache(9);
        cache[0] = 0;          /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)17;
        cache[4] = (seq_t)34;
        cache[5] = (seq_t)92;
        cache[6] = (seq_t)223;
        cache[7] = (seq_t)700;
        cache[8] = (seq_t)1747;
        cache[9] = (seq_t)4931;
        cache_max = 9;
    }
    if (n > cache_cap) {
        grow_cache(n);
    }
    if (n <= cache_max) {
        return cache[n];
    }
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        pos += cache[i-1] * (seq_t)2;
        pos += cache[i-2] * (seq_t)4;
        pos += cache[i-4] * (seq_t)7;
        pos += cache[i-7] * (seq_t)16;
        pos += cache[i-8] * (seq_t)10;
        pos += cache[i-9] * (seq_t)4;

        neg += cache[i-3] * (seq_t)6;
        neg += cache[i-5] * (seq_t)12;
        neg += cache[i-6] * (seq_t)19;

        cache[i] = pos - neg;
    }
    cache_max = n;
    return cache[n];
}
