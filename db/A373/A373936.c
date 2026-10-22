/*
 * A373936  Number of compositions of 7*n-4 into parts 6 and 7.
 * https://oeis.org/A373936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 6*a(n-6) + a(n-7) Initial terms:
 *            a(1)=0,a(2)=0,a(3)=0,a(4)=1,a(5)=5,a(6)=15,a(7)=35
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t cache_size = 0;

static void ensure_capacity(idx_t n)
{
    if (n <= cache_capacity)
        return;
    idx_t new_cap = cache_capacity ? cache_capacity * 2 : 8;
    if (new_cap < n)
        new_cap = n;
    seq_t *new_ptr = NULL;
#ifdef __cplusplus
    new_ptr = new seq_t[new_cap + 1];
#else
    new_ptr = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
#endif
    if (!new_ptr)
        exit(1);
    for (idx_t i = 1; i <= cache_size; ++i)
        new_ptr[i] = cache[i];
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif
    cache = new_ptr;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    static const seq_t init[8] = {0, 0, 0, 0, 1, 5, 15, 35};

    if (n <= 7)
        return init[(size_t)n];

    if (cache_size < 7) {            /* initialize first 7 values */
        ensure_capacity(7);
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = init[(size_t)i];
        cache_size = 7;
    }
    if (n <= cache_size)
        return cache[(size_t)n];

    ensure_capacity(n);
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t res = ((seq_t)7) * cache[(size_t)(i - 1)];
        res -= ((seq_t)21) * cache[(size_t)(i - 2)];
        res += ((seq_t)35) * cache[(size_t)(i - 3)];
        res -= ((seq_t)35) * cache[(size_t)(i - 4)];
        res += ((seq_t)21) * cache[(size_t)(i - 5)];
        res -= ((seq_t)6)  * cache[(size_t)(i - 6)];
        res += cache[(size_t)(i - 7)];
        cache[(size_t)i] = res;
    }
    cache_size = n;
    return cache[(size_t)n];
}
