/*
 * A347855  a(n) = (4*n)!/((2*n)!*(n)!) * (n/3)!/(4*n/3)!.
 * https://oeis.org/A347855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4n,2n) * binomial(2n,n) / binomial(4n/3,n)
 *            Recurrence (valid for all n >= 3): a(n) = 216 * (4n-11) * (4n-1)
 *            * (4n-5) * (4n-7) * a(n-3)
 *            ---------------------------------------------------- n * (n-1) *
 *            (n-2) * (2n-3) Initial values: a(0)=1, a(1)=9, a(2)=189
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Cache for previously computed values */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure cache has space for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n + 1 > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;
        /* initialise new elements to zero */
        for (idx_t i = cache_cap; i <= n; ++i)
            cache[i] = 0;
        cache_cap = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* compute all missing entries up to n */
    for (idx_t k = 0; k <= n; ++k) {
        if (k < cache_cap && cache[k] != 0)
            continue;   /* already computed */

        switch (k) {
            case 0: cache[0] = (seq_t)1; break;
            case 1: cache[1] = (seq_t)9; break;
            case 2: cache[2] = (seq_t)189; break;
            default: {
                seq_t prev = cache[k - 3];
#ifdef __cplusplus
                if constexpr (std::is_integral<seq_t>::value) {
                    unsigned __int128 prod = (unsigned __int128)prev;
                    prod *= 216;
                    prod *= (4 * k - 11);
                    prod *= (4 * k - 1);
                    prod *= (4 * k - 5);
                    prod *= (4 * k - 7);
                    prod /= k;
                    prod /= (k - 1);
                    prod /= (k - 2);
                    prod /= (2 * k - 3);
                    cache[k] = (seq_t)prod;
                } else {
                    seq_t prod = prev;
                    prod *= 216;
                    prod *= (4 * k - 11);
                    prod *= (4 * k - 1);
                    prod *= (4 * k - 5);
                    prod *= (4 * k - 7);
                    prod /= k;
                    prod /= (k - 1);
                    prod /= (k - 2);
                    prod /= (2 * k - 3);
                    cache[k] = prod;
                }
#else
                unsigned __int128 prod = (unsigned __int128)prev;
                prod *= 216;
                prod *= (4 * k - 11);
                prod *= (4 * k - 1);
                prod *= (4 * k - 5);
                prod *= (4 * k - 7);
                prod /= k;
                prod /= (k - 1);
                prod /= (k - 2);
                prod /= (2 * k - 3);
                cache[k] = (seq_t)prod;
#endif
            }
        }
    }
    return cache[n];
}
