/*
 * A314477  Coordination sequence Gal.5.193.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* OEIS sequence A314477
   a(0)=1,a(1)=5,a(2)=8,a(3)=14,a(4)=17,a(5)=22,a(6)=25,a(7)=34,a(8)=35,a(9)=44,a(10)=42,a(11)=51,a(12)=51,a(13)=62,a(14)=60,a(15)=74,a(16)=68,a(17)=79,a(18)=78,
   a(n) = -2*a(n-1)-2*a(n-2)-a(n-3)+a(n-6)+3*a(n-7)+4*a(n-8)+3*a(n-9)+a(n-10)-a(n-13)-2*a(n-14)-2*a(n-15)-a(n-16) for n>18 */

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

static const seq_t init_vals[19] = {
    (seq_t)1,  (seq_t)5,  (seq_t)8,  (seq_t)14, (seq_t)17, (seq_t)22,
    (seq_t)25, (seq_t)34, (seq_t)35, (seq_t)44, (seq_t)42, (seq_t)51,
    (seq_t)51, (seq_t)62, (seq_t)60, (seq_t)74, (seq_t)68, (seq_t)79,
    (seq_t)78
};

seq_t a(idx_t n)
{
    if (cache_size == 0) {
        cache_cap = 19;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 19; ++i) cache[i] = init_vals[i];
        cache_size = 19;
    }

    size_t idx = (size_t)n;
    if (idx < cache_size) return cache[idx];

    if (idx + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < idx + 1) new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_size; i <= idx; ++i) {
        if (i <= 18) {
            cache[i] = init_vals[i];
            continue;
        }
        seq_t pos = cache[i-6];
        pos += 3 * cache[i-7];
        pos += 4 * cache[i-8];
        pos += 3 * cache[i-9];
        pos += cache[i-10];

        seq_t neg = 2 * cache[i-1];
        neg += 2 * cache[i-2];
        neg += cache[i-3];
        neg += cache[i-13];
        neg += 2 * cache[i-14];
        neg += 2 * cache[i-15];
        neg += cache[i-16];

        cache[i] = pos - neg;
    }

    cache_size = idx + 1;
    return cache[idx];
}
