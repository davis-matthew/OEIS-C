/*
 * A355084  E.g.f. A(x) satisfies A(x) = 1 + log(1+x) * A(2*x).
 * https://oeis.org/A355084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} (-1)^(k-1) * 2^(n-k) * (k‑1)! *
 *            binomial(n,k) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* Compute the n-th term of OEIS A355084.  The sequence is defined
   recursively with a(0)=1.  We memoise the computed terms and the
   factorials and powers of two needed in the recurrence. */
seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* a-values */
    static seq_t *fact   = NULL;   /* factorials (k! for k up to n)    */
    static seq_t *pow2   = NULL;   /* powers of 2: 2^i                */
    static size_t cap    = 0;      /* current capacity for arrays      */
    static idx_t  max_computed = 0;/* largest n already computed       */

    /* Allocate or grow arrays to hold up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
            fact  = (seq_t*)malloc(new_cap * sizeof(seq_t));
            pow2  = (seq_t*)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            fact  = (seq_t*)realloc(fact,  new_cap * sizeof(seq_t));
            pow2  = (seq_t*)realloc(pow2,  new_cap * sizeof(seq_t));
        }
        cap = new_cap;
    }

    /* Initialise base case and the first factorial/power of two if this is
       the very first call.  max_computed == 0 indicates that cache[0] may
       be uninitialised. */
    if (max_computed == 0) {
        cache[0] = 1;   /* a(0) */
        fact[0]  = 1;   /* 0!  */
        pow2[0]  = 1;   /* 2^0 */
    }

    /* Pre‑compute factorials and powers of two up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
        pow2[i] = pow2[i - 1] * (seq_t)2;
    }

    /* Compute a(i) for all i > max_computed up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = binomial(i, k);
            term *= pow2[i - k];
            term *= fact[k - 1];
            term *= cache[i - k];
            if ((k & 1) == 0)       /* k even → negative sign */
                neg += term;
            else                    /* k odd  → positive sign */
                pos += term;
        }
        cache[i] = (pos >= neg) ? (pos - neg) : (neg - pos); /* result always positive */
    }

    max_computed = n;
    return cache[n];
}
