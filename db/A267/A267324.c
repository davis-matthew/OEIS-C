/*
 * A267324  Number of elements of S_n with strategic pile of size 4.
 * https://oeis.org/A267324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-4)! * (6*binomial(n-5,3) + 16*binomial(n-5,2) +
 *            16*binomial(n-5,1)) for n>5, a(n)=0 for 1≤n≤5
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached factorials: fact_cache[i] = i! for 0 <= i < fact_cache_len */
static seq_t *fact_cache = NULL;
static size_t fact_cache_len = 0;

/* Ensure factorials up to 'up_to' are computed and cached. */
static void ensure_factorial_cache(idx_t up_to)
{
    /* Allocate the cache on first use. */
    if (fact_cache == NULL) {
        fact_cache = (seq_t*)malloc(sizeof(seq_t));
        if (!fact_cache) return;               /* allocation failure: leave cache empty */
        fact_cache[0] = (seq_t)1;              /* 0! = 1 */
        fact_cache_len = 1;
    }

    /* The cache already covers the requested range. */
    if ((idx_t)fact_cache_len > up_to) return;

    size_t new_len = (size_t)up_to + 1;
    seq_t *tmp = (seq_t*)realloc(fact_cache, new_len * sizeof(seq_t));
    if (!tmp) return;                         /* allocation failure: keep old cache */

    for (idx_t i = (idx_t)fact_cache_len; i <= up_to; ++i) {
        tmp[i] = tmp[i - 1] * (seq_t)i;
    }

    fact_cache_len = new_len;
    fact_cache = tmp;
}

seq_t a(idx_t n)
{
    if (n <= 5) return (seq_t)0;

    idx_t m = n - 5;                           /* m = n-5, used in binomials */

    /* Compute binomial coefficients C(m,1), C(m,2), C(m,3). */
    seq_t c1 = (seq_t)m;                                /* C(m,1) = m */
    seq_t c2 = (seq_t)m * (seq_t)(m - 1) / (seq_t)2;    /* C(m,2) = m*(m-1)/2 */
    seq_t c3 = (seq_t)m * (seq_t)(m - 1) * (seq_t)(m - 2) / (seq_t)6; /* C(m,3) */

    /* Coefficient: 6*C(m,3) + 16*C(m,2) + 16*C(m,1). */
    seq_t coeff = (seq_t)6 * c3 + (seq_t)16 * c2 + (seq_t)16 * c1;

    /* factorial (n-4)! */
    ensure_factorial_cache(n - 4);
    seq_t fact = fact_cache[n - 4];

    return fact * coeff;
}
