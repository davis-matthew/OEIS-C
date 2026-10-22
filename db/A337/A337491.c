/*
 * A337491  Numbers k such that exactly one of 2*k + 3 and 4*k + 3 is prime.
 * https://oeis.org/A337491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4959, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A337491: numbers k such that exactly one of 2*k+3 and 4*k+3 is prime */
/* Algorithm: sequentially test k values, cache results, and return the nth term. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static cache holds the sequence values found so far */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t last_k = 0;  /* highest k already tested */

    if (n == 0) return 0;  /* safeguard for 0 index */

    /* return pre‑computed value when possible */
    if (len >= (size_t)n) return cache[n - 1];

    /* generate elements until we have at least n terms */
    while (len < (size_t)n)
    {
        last_k++;

        /* compute 2*k+3 and 4*k+3 as seq_t */
        seq_t v2 = (seq_t)last_k * (seq_t)2 + (seq_t)3;
        seq_t v4 = (seq_t)last_k * (seq_t)4 + (seq_t)3;

        bool p2 = isPrimeGeneric(v2);
        bool p4 = isPrimeGeneric(v4);

        /* store k if exactly one of the expressions is prime */
        if (p2 ^ p4)
        {
            if (len == cap)
            {
                size_t new_cap = cap ? cap << 1 : 64;
                seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = p;
                cap = new_cap;
            }
            cache[len++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
