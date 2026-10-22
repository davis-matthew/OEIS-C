/*
 * A357703  Expansion of e.g.f. cosh( sqrt(3) * log(1-x) ).
 * https://oeis.org/A357703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-3)*a(n-1) - (n^2-4*n+1)*a(n-2), a(0)=1, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_up_to = 0;

    /* initialisation */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));   /* space for a(0), a(1) */
        if (!cache) return 0;                         /* out‑of‑memory guard */
        cache[0] = (seq_t)1;                          /* a(0) */
        cache[1] = (seq_t)0;                          /* a(1) */
        cached_up_to = 1;                             /* max index computed so far */
    }

    /* answer from cache if already known */
    if (n <= cached_up_to)
        return cache[n];

    /* expand cache to cover up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;               /* still out‑of‑memory guard */
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        long long coeff_ll = (long long)i * i - 4LL * (long long)i + 1;
        idx_t coeff_abs = (idx_t)(coeff_ll < 0 ? -coeff_ll : coeff_ll);

        seq_t prod2 = coeff_abs * cache[i - 2];
        idx_t  coef1 = (idx_t)(2 * i - 3);
        seq_t prod1 = coef1 * cache[i - 1];

        if (coeff_ll < 0)
            cache[i] = prod1 + prod2;
        else
            cache[i] = prod1 - prod2;
    }

    cached_up_to = n;
    return cache[n];
}
