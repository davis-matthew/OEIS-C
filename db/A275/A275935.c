/*
 * A275935  Shifts 5 places left under binomial transform.
 * https://oeis.org/A275935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, eigen
 * Formula:   a(n) = sum_{i=0}^{n-5} binom(n-5,i) * a(i) for n>=5, with
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=1
 * Range:     u8 n <= 15, u16 n <= 19, u32 n <= 28, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_calc = 4;  /* Highest index already computed */

    /* Base values for indices 0..4 */
    const seq_t base_vals[5] = { (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1 };

    if (n < 5)
        return base_vals[n];

    /* Allocate cache on first use */
    if (cache == NULL) {
        cache_size = 5;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* initialize cache with base values */
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = base_vals[i];
    }

    /* Grow cache array if necessary */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* Newly added slots are zero‑filled (not used until computed) */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Compute terms that have not yet been filled */
    for (idx_t m = last_calc + 1; m <= n; ++m) {
        /* m is at least 5 here */
        idx_t k = m - 5;                     /* compute sum over i=0..k */
        seq_t sum = 0;
        seq_t binom = 1;                     /* binom(k,0) */
        for (idx_t i = 0; i <= k; ++i) {
            sum += binom * cache[i];
            if (i < k) {
                binom = binom * (seq_t)(k - i) / (seq_t)(i + 1);
            }
        }
        cache[m] = sum;
    }
    last_calc = n;

    return cache[n];
}
