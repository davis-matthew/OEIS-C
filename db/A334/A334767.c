/*
 * A334767  a(n) = Product_{k=1..n} d(2*k), where d() is the number of
 *          divisors function A000005.
 * https://oeis.org/A334767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} d(2k) (d = divisor count)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Static cache of partial products: prod[0] = 1, prod[i] = a(i) */
    static seq_t *prod = NULL;
    static size_t prod_cap = 0;
    static idx_t max_n = 0;

    /* Ensure prod[0] exists */
    if (prod == NULL) {
        prod_cap = 1;
        prod = (seq_t *)malloc(prod_cap * sizeof(seq_t));
        if (prod == NULL) return (seq_t)0;   /* allocation failed */
        prod[0] = (seq_t)1;
    }

    if (n > max_n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > prod_cap) {
            /* grow array */
            seq_t *tmp = (seq_t *)realloc(prod, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* allocation failed */
            prod = tmp;
            prod_cap = new_cap;
        }

        /* compute from max_n+1 up to n */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            uint64_t d = divisor_count_u64((uint64_t)(2 * k));
            prod[k] = prod[k - 1] * (seq_t)d;
        }
        max_n = n;
    }

    return prod[n];
}
