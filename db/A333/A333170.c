/*
 * A333170  a(n) = Sum_{k=0..n} phi(k^2 + 1), where phi is the Euler totient
 *          function (A000010).
 * https://oeis.org/A333170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} φ(k^2 + 1)
 * Range:     u8 n <= 9, u16 n <= 65, u32 n <= 2677, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* phi is the Euler totient function from utils/phi.h */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/phi.h"

/* Computes a(n) for any n >= 0.  The result is cached so that
   repeated calls are O(1) after the first computation up to the
   maximum n seen so far. */
seq_t a(idx_t n)
{
    /* static cache of cumulative sums */
    static seq_t  *cum      = NULL;     /* array of values cum[0..max_index] */
    static idx_t   max_index = 0;       /* largest index computed so far */
    static size_t  capacity  = 0;       /* allocated size of cum[]            */

    /* Initialise cache on first call */
    if (cum == NULL) {
        capacity   = 1;
        cum        = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cum == NULL) return (seq_t)0;              /* out of memory */
        cum[0]     = phi((idx_t)1);        /* φ(0^2+1) = φ(1) = 1 */
        max_index  = 0;
    }

    /* Expand cache if a larger index is requested */
    if (n > max_index) {
        /* Ensure enough space – double until capacity >= n+1 */
        if ((size_t)(n + 1) > capacity) {
            size_t new_cap = capacity ? capacity << 1 : 1;
            if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
            seq_t *tmp = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;          /* out of memory */
            cum       = tmp;
            capacity  = new_cap;
        }

        seq_t sum = cum[max_index];
        for (idx_t k = max_index + 1; k <= n; ++k) {
            seq_t m        = (seq_t)k * (seq_t)k + (seq_t)1;
            seq_t phi_val  = phi((idx_t)m);
            sum           += phi_val;
            cum[k]         = sum;
        }
        max_index = n;
    }

    return cum[n];
}
