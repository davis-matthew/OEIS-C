/*
 * A269694  Product of first n nonzero Jacobsthal numbers (A001045).
 * https://oeis.org/A269694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=1..n} J(k) where * J(1)=1, J(2)=1,
 *            J(n)=J(n-1)+2*J(n-2) (Jacobsthal numbers)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n<1 */

    static seq_t *product = NULL;  /* a(i) for i=1..cap */
    static seq_t *jacob    = NULL;  /* J(i) for i=1..cap */
    static idx_t  cache_len = 0;    /* highest index computed */
    static idx_t  cache_cap = 0;    /* allocated capacity (>= cache_len) */

    /* enlarge capacity if needed */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap > 0) ? cache_cap : 16;
        while (new_cap < n) new_cap <<= 1;
        product = (seq_t*)realloc(product, (size_t)(new_cap + 1) * sizeof(seq_t));
        jacob   = (seq_t*)realloc(jacob,   (size_t)(new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise if first call */
    if (cache_len == 0) {
        jacob[1] = (seq_t)1;
        product[1] = jacob[1];
        cache_len = 1;
    }
    if (n >= 2 && cache_len < 2) {
        jacob[2] = (seq_t)1;
        product[2] = product[1] * jacob[2];
        cache_len = 2;
    }

    /* compute remaining terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 2) {
            jacob[2] = (seq_t)1;
        } else {
            jacob[i] = jacob[i - 1] + (seq_t)2 * jacob[i - 2];
        }
        product[i] = product[i - 1] * jacob[i];
    }

    cache_len = n;
    return product[n];
}
