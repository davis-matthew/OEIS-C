/*
 * A358246  Number of n-regular, N_0-weighted pseudographs on 2 vertices with
 *          total edge weight 6, up to isomorphism.
 * https://oeis.org/A358246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) for n >= 13 Initial values:
 *            a(1)=1, a(2)=8, a(3)=23, a(4)=55, a(5)=92, a(6)=147, a(7)=196,
 *            a(8)=260, a(9)=313, a(10)=380, a(11)=434, a(12)=502
 * Range:     u8 n <= 7, u16 n <= 1078, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values
       cache[1] holds a(1); index 0 is unused   */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated array size */
    static size_t max_computed = 0; /* greatest index that has a valid value */

    /* grow cache if necessary */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < cap; ++i)
            cache[i] = 0;
    }
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cap = new_cap;
    }

    /* If value already computed, return it */
    if (n <= max_computed)
        return cache[n];

    /* Base values for n = 1 .. 12 */
    static const unsigned long long base_vals[13] = {
        0ULL, 1ULL, 8ULL, 23ULL, 55ULL, 92ULL, 147ULL,
        196ULL, 260ULL, 313ULL, 380ULL, 434ULL, 502ULL
    };
    if (max_computed < 12) {
        size_t up_to = (size_t)n < 12 ? n : 12;
        for (size_t i = max_computed + 1; i <= up_to; ++i)
            cache[i] = (seq_t)base_vals[i];
        if (up_to > max_computed)
            max_computed = up_to;
    }

    /* Recurrence for n >= 13 */
    for (size_t k = max_computed + 1; k <= (size_t)n; ++k) {
        cache[k] = cache[k - 1] + cache[k - 2] - cache[k - 3];
    }
    max_computed = (size_t)n;

    return cache[n];
}
