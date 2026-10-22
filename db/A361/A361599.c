/*
 * A361599  Expansion of e.g.f. exp( x/(1-x)^3 ) / (1-x).
 * https://oeis.org/A361599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2*n - 1)*a(n-1) - (n-1)*(6*n - 11)*a(n-2) +
 *            (n-2)*(n-1)*(4*n - 9)*a(n-3) - (n-3)^2*(n-2)*(n-1)*a(n-4), with
 *            a(0)=1, a(1)=2, a(2)=11, a(3)=88
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t cached_until = (idx_t)-1; /* largest index computed */

    /* Initialise cache if necessary */
    if (cache == NULL) {
        /* Ensure space for at least indices 0..3 if needed */
        cache_size = (size_t)((n >= 3) ? 4 : (size_t)(n + 1));
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* Base values */
        cache[0] = (seq_t)1;
        if (cache_size > 1) cache[1] = (seq_t)2;
        if (cache_size > 2) cache[2] = (seq_t)11;
        if (cache_size > 3) cache[3] = (seq_t)88;
        cached_until = (cache_size >= 4) ? 3 : (idx_t)(cache_size - 1);
    }

    /* Return immediately if already computed */
    if (n <= cached_until)
        return cache[n];

    /* Ensure array is large enough */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: in competitive settings we can abort,
               but here we simply leak memory (not required by contract). */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
        /* The new slots are uninitialised; they'll be written below */
    }

    /* Compute values from cached_until+1 up to n */
    for (idx_t i = cached_until + 1; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        seq_t a_nm3 = cache[i - 3];
        seq_t a_nm4 = cache[i - 4];

        /* Compute each term with seq_t arithmetic to avoid overflow in intermediate idx_t. */
        seq_t term1 = (seq_t)2 *
                      (((seq_t)2 * (seq_t)i) - (seq_t)1) *
                      a_nm1;

        seq_t term2 = ((seq_t)(i - 1)) *
                      (((seq_t)6 * (seq_t)i) - (seq_t)11) *
                      a_nm2;

        seq_t term3 = ((seq_t)(i - 2)) *
                      ((seq_t)(i - 1)) *
                      (((seq_t)4 * (seq_t)i) - (seq_t)9) *
                      a_nm3;

        seq_t term4 = ((seq_t)(i - 3)) *
                      ((seq_t)(i - 3)) *
                      ((seq_t)(i - 2)) *
                      ((seq_t)(i - 1)) *
                      a_nm4;

        cache[i] = term1 - term2 + term3 - term4;
    }

    cached_until = n;
    return cache[n];
}
