/*
 * A299816  Number of nX3 0..1 arrays with every element equal to 0, 2, 3, 4,
 *          5, 6 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A299816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 4*a(n-2) - 39*a(n-3) - 35*a(n-4) + 109*a(n-5)
 *            * + 37*a(n-6) - 129*a(n-7) + 69*a(n-8) + 95*a(n-9) - 110*a(n-10)
 *            * + 6*a(n-11) - 20*a(n-12) - 28*a(n-13) + 6*a(n-14), n>16 *
 *            Initial values: a(1)=1, a(2)=13, a(3)=39, a(4)=202, a(5)=894, *
 *            a(6)=4507, a(7)=22684, a(8)=116651, a(9)=605...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static bool cache_initialized = false;
static const seq_t init_vals[16] = {
    1ULL, 13ULL, 39ULL, 202ULL, 894ULL, 4507ULL, 22684ULL, 116651ULL,
    605727ULL, 3159382ULL, 16537678ULL, 86697810ULL, 454970673ULL,
    2388855177ULL, 12546612125ULL, 65907887547ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n>=1 expected */

    /* initialise cache for first 16 values */
    if (!cache_initialized) {
        cache_size = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        for (size_t i = 0; i < 16; ++i) cache[i] = init_vals[i];
        cache_initialized = true;
    }

    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* extend cache up to n */
    idx_t old_size = cache_size;
    cache_size = n;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_size);

    for (idx_t i = old_size; i < n; ++i) {
        /* i corresponds to term a(i+1) */
        if (i < 16) {
            cache[i] = init_vals[i];
        } else {
            /* compute using the recurrence, avoiding signed intermediate */
            seq_t pos = 6 * cache[i - 1]      /* a(i)   */
                      + 4 * cache[i - 2]      /* a(i-1) */
                      + 109 * cache[i - 5]    /* a(i-4) */
                      + 37 * cache[i - 6]     /* a(i-5) */
                      + 69 * cache[i - 8]     /* a(i-7) */
                      + 95 * cache[i - 9]     /* a(i-8) */
                      + 6 * cache[i - 11]     /* a(i-10) */
                      + 6 * cache[i - 14];    /* a(i-13) */

            seq_t neg = 39 * cache[i - 3]      /* a(i-2)  */
                    + 35 * cache[i - 4]        /* a(i-3)  */
                    + 129 * cache[i - 7]       /* a(i-6)  */
                    + 110 * cache[i - 10]      /* a(i-9)  */
                    + 20 * cache[i - 12]       /* a(i-11) */
                    + 28 * cache[i - 13];      /* a(i-12) */

            cache[i] = pos - neg;
        }
    }
    return cache[n - 1];
}
