/*
 * A247405  Number of length 1+4 0..n arrays with no disjoint pairs in any
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) -2*a(n-2) - a(n-3) + a(n-4) -2*a(n-5) +4*a(n-6)
 *            -2*a(n-7) + a(n-8) - a(n-9) -2*a(n-10) +3*a(n-11) - a(n-12), for
 *            n ≥ 13 with initial values a(1)=0, a(2)=40, a(3)=200, a(4)=760,
 *            a(5)=2280, a(6)=5640, a(7)=12120, a(8)=23800, a(9)=43040,
 *            a(10)=73840, a(11)=120240, a(12)=188040
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 85, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t cache_size = 0;      /* largest n stored */
    static size_t cache_cap  = 0;      /* capacity of cache */

    /* initial 12 terms (index 0 unused) */
    static const seq_t init[13] =
        {0ULL, 0ULL, 40ULL, 200ULL, 760ULL, 2280ULL,
         5640ULL, 12120ULL, 23800ULL, 43040ULL,
         73840ULL, 120240ULL, 188040ULL};

    /* return directly for n ≤ 12 */
    if (n <= 12)
        return init[n];

    /* allocate and initialise cache if first call */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        if (cache_cap < 13) cache_cap = 13;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 1; i <= 12; ++i)
            cache[i] = init[i];
        cache_size = 12;
    }

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        /* separate positive and negative contributions to avoid
           intermediate underflow on unsigned types */
        seq_t sum_pos = 3 * cache[i - 1] + cache[i - 4] +
                        4 * cache[i - 6] + cache[i - 8] +
                        3 * cache[i - 11];

        seq_t sum_neg = 2 * cache[i - 2] + cache[i - 3] +
                        2 * cache[i - 5] + 2 * cache[i - 7] +
                        cache[i - 9] + 2 * cache[i - 10] +
                        cache[i - 12];

        cache[i] = sum_pos - sum_neg;
    }

    cache_size = (size_t)n;
    return cache[n];
}
