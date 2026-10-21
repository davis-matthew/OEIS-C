/*
 * A267212  Total number of ON (black) cells after n iterations of the "Rule
 *          109" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)-a(n-5)-a(n-7)+a(n-8) for n>7; Initial
 *            values: a(0)=1, a(1)=2, a(2)=7, a(3)=9, a(4)=16, a(5)=23,
 *            a(6)=33, a(7)=38
 * Range:     u8 n <= 19, u16 n <= 333, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static const seq_t base_vals[8] = { (seq_t)1, (seq_t)2, (seq_t)7, (seq_t)9,
                                   (seq_t)16, (seq_t)23, (seq_t)33, (seq_t)38 };

seq_t a(idx_t n)
{
    if (n < 8)
        return base_vals[n];

    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 8);
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = base_vals[i];
        cache_len = 8;
    }

    if (n >= cache_len) {
        size_t target = (size_t)n;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (target + 1));
        size_t start = (size_t)cache_len;
        for (size_t i = start; i <= target; ++i) {
            cache[i] = cache[i - 1] + cache[i - 3] + cache[i - 8]
                       - cache[i - 5] - cache[i - 7];
        }
        cache_len = target + 1;
    }

    return cache[n];
}
