/*
 * A309715  Number of even parts appearing among the third largest parts of
 *          the partitions of n into 4 parts.
 * https://oeis.org/A309715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-6) - 2*a(n-7) + 2*a(n-8) -
 *            2*a(n-9) + a(n-10) - a(n-14) + 2*a(n-15) - a(n-16) with initial
 *            values a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=0,
 *            a(7)=1, a(8)=2, a(9)=3, a(10)=4, a(11)=5, a(12)=6, a(13)=8,
 *            a(14)=10, a(15)=13
 * Range:     u8 n <= 41, u16 n <= 265, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise cache if necessary */
    if (cache_size == 0) {
        /* initial size at least 16 */
        cache_size = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) return 0; /* allocation failed */
        /* initial terms */
        static const seq_t init[16] = {
            0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 8, 10, 13
        };
        for (size_t i = 0; i < cache_size; ++i) cache[i] = init[i];
    }

    /* grow cache up to n */
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failed */
        cache = tmp;
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            if (i <= 15) {
                /* already initialized */
                continue;
            }
            seq_t val = 0;
            val += 2 * cache[i - 1];
            val -=     cache[i - 2];
            val +=     cache[i - 6];
            val -= 2 * cache[i - 7];
            val += 2 * cache[i - 8];
            val -= 2 * cache[i - 9];
            val +=     cache[i -10];
            val -=     cache[i -14];
            val += 2 * cache[i -15];
            val -=     cache[i -16];
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
