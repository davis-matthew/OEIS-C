/*
 * A251740  8-step Fibonacci sequence starting with 0,0,0,0,0,1,0,0.
 * https://oeis.org/A251740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+8) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6)+a(n+7),
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=1,a(6)=0,a(7)=0
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 72, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = 0;
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;

seq_t a(idx_t n)
{
    /* Handle the first eight terms directly. */
    if (n < 8) {
        static const seq_t init[8] = {0, 0, 0, 0, 0, 1, 0, 0};
        return init[n];
    }

    /* First call: allocate cache and store the known initials. */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        static const seq_t init[8] = {0, 0, 0, 0, 0, 1, 0, 0};
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
        cache_size = 8;
    }

    /* Extend cache up to n if necessary. */
    if (n >= cache_size) {
        idx_t need = n + 1;
        if (need > cache_cap) {
            idx_t new_cap = cache_cap;
            while (new_cap < need) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        for (idx_t m = cache_size; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 8; ++k) sum += cache[m - k];
            cache[m] = sum;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
