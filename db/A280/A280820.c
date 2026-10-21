/*
 * A280820  Partial sums of A001783.
 * https://oeis.org/A280820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} A001783(i)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A001783(i) = product of integers 1 <= k <= i with gcd(k,i)=1 */

/* Computes the sequence.  Uses memoisation so each n is
   computed only once.  Holds product and partial–sum arrays.
   Handles arbitrary‑size seq_t (big‑int) or fixed building blocks. */

#include <stdint.h>
#include <stdlib.h>

/* Helper Euclidean gcd for idx_t, which is uint64_t */
static inline idx_t _gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Static memoisation tables */
    static seq_t *prod   = NULL; /* product of totatives for each i */
    static seq_t *sum    = NULL; /* partial sums */
    static size_t cap    = 0;    /* allocated capacity */
    static idx_t  len    = 0;    /* number of computed terms */

    /* Allocate or extend tables as required */
    if (n > cap) {
        size_t new_cap = (cap > 0) ? cap * 2 : 64;
        while (new_cap < n) new_cap *= 2;
        prod = (seq_t *)realloc(prod, new_cap * sizeof(seq_t));
        sum  = (seq_t *)realloc(sum,  new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t idx = len; idx < n; ++idx) {
        idx_t i = idx + 1;                     /* current n */
        seq_t prod_i = (seq_t)1;                /* product of totatives of i */

        for (idx_t k = 1; k <= i; ++k) {
            if (_gcd_u64(k, i) == 1) {
                prod_i *= (seq_t)k;            /* multiply by the coprime k */
            }
        }
        prod[idx] = prod_i;
        sum[idx]  = ((idx > 0) ? sum[idx - 1] : (seq_t)0) + prod_i;
    }
    len = n;

    return sum[n - 1];
}
