/*
 * A377826  E.g.f. satisfies A(x) = (1 + x) * exp(x * A(x)).
 * https://oeis.org/A377826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)^(k-1) * binomial(k+1,n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation evaluates the sum directly, caching factorials
 * and the powers (k+1)^(k-1) for all k up to the maximum n requested. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute binomial coefficient n choose k multiplicatively.
   This works for any seq_t that supports + - * / % with integer constants. */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)      /* use symmetry */
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Generic power for small exponents (k <= 3) */
static inline seq_t pow_seq(seq_t a, int k)
{
    seq_t r = 1;
    while (k)
    {
        if (k & 1)
            r *= a;
        if (k >>= 1)
            a *= a;
    }
    return r;
}

seq_t a(idx_t n) {
    /* Static cache for factorials and powers */
    static seq_t *fact = NULL;   /* fact[i] = i!  (seq_t) */
    static seq_t *pow  = NULL;   /* pow[i]  = (i+1)^(i-1)  (seq_t) */
    static idx_t    cache_n = 0; /* highest n already cached */

    /* Initialise cache on first call */
    if (fact == NULL) {
        fact = mem_new(seq_t, (n + 1));
        pow  = mem_new(seq_t, (n + 1));
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
        seq_t *new_fact = mem_resize(seq_t, fact, (n + 1));
        seq_t *new_pow  = mem_resize(seq_t, pow, (n + 1));
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
