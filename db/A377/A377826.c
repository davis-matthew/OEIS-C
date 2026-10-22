/*
 * A377826  E.g.f. satisfies A(x) = (1 + x) * exp(x * A(x)).
 * https://oeis.org/A377826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(k-1) * binomial(k+1,n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation evaluates the sum directly, caching factorials
 * and the powers (k+1)^(k-1) for all k up to the maximum n requested. */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/binomial.h"   /* seq_t binomial(idx_t, idx_t) */
#include "../utils/pow_seq.h"    /* seq_t pow_seq(seq_t, int) */

seq_t a(idx_t n) {
    /* Static cache for factorials and powers */
    static seq_t *fact = NULL;   /* fact[i] = i!  (seq_t) */
    static seq_t *pow  = NULL;   /* pow[i]  = (i+1)^(i-1)  (seq_t) */
    static idx_t    cache_n = 0; /* highest n already cached */

    /* Initialise cache on first call */
    if (fact == NULL) {
        fact = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        pow  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!fact || !pow) {
            /* On allocation failure return 0 (should not happen in tests). */
            return (seq_t)0;
        }
        fact[0] = (seq_t)1;
        pow[0]  = (seq_t)1;          /* By convention, 1^(-1) = 1 for k = 0 */
        cache_n = 0;
    }

    /* Expand the cache if a larger n is requested */
    if (n > cache_n) {
        seq_t *new_fact = (seq_t *)realloc(fact, (n + 1) * sizeof(seq_t));
        seq_t *new_pow  = (seq_t *)realloc(pow,  (n + 1) * sizeof(seq_t));
        if (!new_fact || !new_pow) {
            /* Allocation failure – abort (should not occur). */
            return (seq_t)0;
        }
        fact = new_fact;
        pow  = new_pow;

        /* Compute new factorials */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }

        /* Compute the powers (i+1)^(i-1) */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            if (i == 0) {
                pow[0] = (seq_t)1;
            } else {
                /* pow_seq base = i+1, exponent = i-1 (non‑negative) */
                pow[i] = pow_seq((seq_t)(i + 1), (int)(i - 1));
            }
        }
        cache_n = n;
    }

    /* Evaluate the defining sum */
    seq_t result = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        /* Skip terms where binomial is zero */
        if (n - k > k + 1) continue;

        seq_t term = fact[n] / fact[k];          /* n! / k! */
        term = term * pow[k];                    /* * (k+1)^(k-1) */
        term = term * binomial(k + 1, n - k);    /* * C(k+1, n-k) */
        result += term;
    }
    return result;
}
