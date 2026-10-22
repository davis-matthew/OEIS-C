/*
 * A370458  Partial products of A051179.
 * https://oeis.org/A370458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n} (2^{2^k} - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

/* Returns the nth term of OEIS A370458:  a(0) = 1, a(1) = 3, a(2) = 45, ... */
seq_t a(idx_t n)
{
    /* Cache of already computed partial products */
    static seq_t *prod = NULL;          /* array of a(k) values */
    static size_t prod_cap = 0;         /* current capacity of prod   */
    static size_t prod_len = 0;         /* number of computed values   */
    static seq_t last_t = 0;            /* last t_k = 2^{2^k} - 1    */

    /* Initialise cache on first call */
    if (prod == NULL) {
        /* Ensure space for at least a(0) */
        ensure_capacity(1, &prod_cap, (void **)&prod, sizeof(seq_t));
        prod[0] = 1;       /* a(0) */
        last_t = 1;        /* t_0 = 2^{2^0} - 1 = 1   */
        prod_len = 1;      /* one value computed so far */
    }

    /* Convert idx_t to size_t safely (n fits in size_t for our usage) */
    size_t target = (size_t)n;

    /* Compute up to the requested term if needed */
    if (target >= prod_len) {
        /* Allocate enough space for a(0)..a(target) */
        ensure_capacity(target + 1, &prod_cap, (void **)&prod, sizeof(seq_t));

        /* Compute new partial products */
        for (size_t idx = prod_len; idx <= target; ++idx) {
            /* Compute next t_k = t_{k-1}^2 + 2*t_{k-1}  */
            seq_t next_t = last_t * last_t + 2 * last_t;
            /* Update a(idx) = a(idx-1) * t_k */
            prod[idx] = prod[idx - 1] * next_t;
            /* Advance last_t */
            last_t = next_t;
        }
        prod_len = target + 1;  /* all terms 0..target are now available */
    }

    return prod[target];
}
