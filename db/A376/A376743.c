/*
 * A376743  Number of permutations (p(1),p(2),...,p(n)) of (1,2,...,n) such
 *          that p(i)-i is in {-2,-1,4} for all i=1,...,n.
 * https://oeis.org/A376743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = a(n-3)+a(n-4)+a(n-5)+2*a(n-6)+a(n-7)
 *            -2*a(n-9)-2*a(n-10)-a(n-12)+a(n-15) with initial values: a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1, a(6)=1, a(7)=1, a(8)=1,
 *            a(9)=1, a(10)=1, a(11)=5, a(12)=5, a(13)=6, a(14)=8, a(15)=11
 * Range:     u8 n <= 24, u16 n <= 40, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[16] = {
    1, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 5,
    5, 6, 8, 11
};

static seq_t *cache = NULL;          /* array holding computed terms */
static size_t cache_size = 0;        /* allocated size of cache   */
static size_t computed_max = 0;      /* largest n for which a(n) is known */

/* ensure that cache contains values up to and including n */
static void ensure_cache(size_t n)
{
    /* allocate or grow cache if necessary */
    if (cache == NULL) {
        cache_size = (n < 32) ? 32 : n * 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);   /* abort on allocation failure */
        /* initialise with base values up to 15 */
        for (size_t i = 0; i < 16; ++i) cache[i] = init_vals[i];
        computed_max = 15;
    } else if (n > cache_size - 1) {
        size_t new_size = cache_size;
        while (new_size <= n) new_size <<= 1;      /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    if (n > computed_max) {
        for (size_t i = computed_max + 1; i <= n; ++i) {
            /* recurrence uses indices i-3 … i-15 which are all already present */
            seq_t val = cache[i - 3] + cache[i - 4] + cache[i - 5]
                      + 2 * cache[i - 6] + cache[i - 7];
            val -= 2 * cache[i - 9];
            val -= 2 * cache[i - 10];
            val -=    cache[i - 12];
            val +=    cache[i - 15];
            cache[i] = val;
        }
        computed_max = n;
    }
}

seq_t a(idx_t n)
{
    /* force cache up to the requested index */
    ensure_cache((size_t)n);
    return cache[(size_t)n];
}
