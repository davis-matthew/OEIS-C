/*
 * A399062  Expansion of e.g.f. exp(x * cosh(x)^3).
 * https://oeis.org/A399062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A399062: Expansion of e.g.f. exp(x * cosh(x)^3).  
   Recurrence: 
     a(0) = 1; 
     a(n) = Sum_{k=0..floor((n-1)/2)} (2*k+1) * binomial(n-1,2*k) 
            * a(n-1-2*k) * ((3^(2*k)+3)/4)   for n >= 1.
   The factor ((3^(2*k)+3)/4) equals A054879(k). */

#include <stddef.h>
#include <stdlib.h>

/* Provided utilities */
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  max_done   = 0;

    /* Ensure cache is large enough for index n */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* memory error – return 0 (unlikely) */
        /* Zero the new portion – new_cap > cache_cap */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise a(0) at first use */
    if (cache[0] == 0) cache[0] = 1;

    /* Return cached value if already computed */
    if (n <= max_done) return cache[n];

    /* Compute all values up to n */
    for (idx_t i = max_done + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t maxk = (i - 1) / 2;
        for (idx_t k = 0; k <= maxk; ++k) {
            /* a(i-1-2k) is already computed */
            seq_t term = cache[i - 1 - 2 * k];

            /* Multiply by (2k+1) */
            term *= (2 * k + 1);

            /* Multiply by binomial(i-1, 2k) */
            term *= binomial(i - 1, 2 * k);

            /* Multiply by A054879(k) = (3^(2k)+3)/4 */
            seq_t pow3 = pow_uint(3, 2 * k);          /* 3^(2k) */
            seq_t A    = (pow3 + 3) / 4;              /* integer division OK */
            term *= A;

            sum += term;
        }
        cache[i] = sum;
    }

    max_done = n;
    return cache[n];
}
