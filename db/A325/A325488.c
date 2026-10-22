/*
 * A325488  Dimensions of space of harmonic polynomials of each degree
 *          invariant under the full icosahedral group.
 * https://oeis.org/A325488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) + a(n-10) - a(n-16) for n > 15, with initial
 *            values a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1,
 *            a(7)=0, a(8)=0, a(9)=0, a(10)=1, a(11)=0, a(12)=1, a(13)=0,
 *            a(14)=0, a(15)=0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* initial 16 terms a(0) to a(15) */
static const seq_t init_terms[16] = {
    1, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 1,
    0, 1, 0, 0, 0
};

seq_t a(idx_t n)
{
    /* initialize cache with the first 16 values */
    if (!cache) {
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init_terms[i];
    }

    /* extend and fill cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = cache[i - 6] + cache[i - 10];
            cache[i] = sum - cache[i - 16];
        }
        cache_size = new_size;
    }

    return cache[n];
}
