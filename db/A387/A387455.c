/*
 * A387455  Partial sums of squares of A000975.
 * https://oeis.org/A387455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + a(n-2) - 25*a(n-3) + 16*a(n-4) + 20*a(n-5) -
 *            16*a(n-6) Initial values: a(0)=0, a(1)=1, a(2)=5, a(3)=30,
 *            a(4)=130, a(5)=571
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* initialise cache */
    if (cache_size == 0) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;              /* out of memory – undefined but safe */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)30;
        cache[4] = (seq_t)130;
        cache[5] = (seq_t)571;
    }

    if (n < cache_size)
        return cache[n];

    /* extend cache up to n */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0;  /* out of memory – undefined but safe */
    cache = tmp;
    cache_size = new_cap;

    /* compute remaining terms */
    for (idx_t k = 6; k <= n; ++k) {
        seq_t t1 = 5 * cache[k - 1];
        seq_t t2 = cache[k - 2];
        seq_t t3 = 25 * cache[k - 3];
        seq_t t4 = 16 * cache[k - 4];
        seq_t t5 = 20 * cache[k - 5];
        seq_t t6 = 16 * cache[k - 6];

        seq_t val = t1 + t2 + t4 + t5;
        val -= t3;
        val -= t6;
        cache[k] = val;
    }

    return cache[n];
}
