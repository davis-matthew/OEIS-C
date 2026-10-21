/*
 * A243107  Number of terms in a bordered skew determinant.
 * https://oeis.org/A243107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,n-2k) * A002370(k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the recurrence derived from the EGF:
   a_{m+1} = a_m + m^2 a_{m-1} - m(m-1) a_{m-2} - (m(m-1)(m-2)/2) a_{m-3}
   with a_0=1, a_1=1, a_2=2, a_3=4. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_until = 0;   /* highest index already computed */

    /* Base values for a(0)..a(3) */
    static const seq_t base_values[4] = {1, 1, 2, 4};

    /* Allocate cache if needed */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failure */
        /* Initialize base terms */
        for (idx_t i = 0; i < 4; ++i)
            cache[i] = base_values[i];
        computed_until = 3;
    }

    /* Grow cache if caller asks for larger n */
    if (n + 1 > cache_size) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure */
    }

    /* If requested value already computed, return it */
    if (n <= computed_until)
        return cache[n];

    /* Compute new terms up to n */
    for (idx_t i = computed_until + 1; i <= n; ++i) {
        idx_t m = i - 1;                /* we compute a_{m+1} = a_i */

        seq_t term2 = cache[m - 1] * ((seq_t)m * m);                             /* m^2 * a_{m-1} */
        seq_t term3 = cache[m - 2] * ((seq_t)m * (m - 1));                        /* m(m-1) * a_{m-2} */
        seq_t term4 = cache[m - 3] *
                      (((seq_t)m * (m - 1) * (m - 2)) / 2);                      /* (m(m-1)(m-2)/2) * a_{m-3} */

        cache[i] = cache[m] + term2 - term3 - term4;
    }

    computed_until = n;
    return cache[n];
}
