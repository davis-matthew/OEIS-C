/*
 * A376810  Expansion of 1/sqrt(1 - 4*x/(1 - x^2)^2).
 * https://oeis.org/A376810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(2*n-3*k-1,k) *
 *            binomial(2*n-4*k,n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static seq_t *a_cache = NULL;           /* cached values */
static size_t a_cache_size = 0;         /* number of elements currently cached (>=0) */

/* Compute the n-th term of OEIS A376810 */
seq_t a(idx_t n)
{
    /* Ensure the cache contains the first term a(0) = 1 */
    if (a_cache_size == 0) {
        a_cache = mem_new(seq_t, 1);
        if (!a_cache) return 0;             /* allocation failure */
        a_cache[0] = 1;
        a_cache_size = 1;
    }

    /* If the requested term is already cached, return it */
    if (n < (idx_t)a_cache_size) {
        return a_cache[n];
    }

    /* Extend the cache to hold terms up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = mem_resize(seq_t, a_cache, new_size);
    if (!tmp) return 0;                 /* allocation failure */
    a_cache = tmp;

    /* Compute missing terms */
    for (idx_t m = (idx_t)a_cache_size; m <= n; ++m) {
        seq_t sum = 0;
        idx_t k_max = m / 2;            /* floor(m/2) */
        for (idx_t k = 0; k <= k_max; ++k) {
            seq_t b1 = binomial(2 * m - 3 * k - 1, k);
            seq_t b2 = binomial(2 * m - 4 * k, m - 2 * k);
            sum += b1 * b2;
        }
        a_cache[m] = sum;
    }

    a_cache_size = new_size;
    return a_cache[n];
}
