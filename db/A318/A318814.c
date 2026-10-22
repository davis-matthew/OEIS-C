/*
 * A318814  Expansion of e.g.f. Product_{k>=1} ((1 + x^k)/(1 -
 *          x^k))^(sigma(k)/k).
 * https://oeis.org/A318814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A318814:
   a(n) are the coefficients of the exponential generating function
   \prod_{k\ge1}\Bigl(\frac{1+x^k}{1-x^k}\Bigr)^{\sigma(k)/k}.
   We use the equivalent recurrence
         a(n) = \sum_{k=1}^{n} a(n-k) * (n-1)!/(n-k)! * λ_k
   where
         λ_k = 2 * Σ_{d|k} σ(d)                          if k odd
         λ_k = 2 * (Σ_{d|k} σ(d) - Σ_{d|k/2} σ(d))        if k even
   Here σ(k) is the sum of divisors of k.
   The values λ_k are all < 2^32 for k <= 10⁴, so they fit into
   an unsigned int and can be used with the provided mul_small
   routine.  The coefficient a(0) is defined to be 1.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) -> seq_t */
#include "../utils/mul_small.h" /* mul_small(seq_t a, unsigned int m) -> seq_t */

/* Cache for previously computed a(n) values, indexed by n. */
static seq_t *a_cache = NULL;     /* holds a(0) .. a(cap-1) */
static idx_t  a_cap   = 0;        /* size of allocated cache */

/* Temporary arrays for divisor sums. */
static unsigned long long *sigma_arr   = NULL; /* σ(k) */
static unsigned long long *sum_sigma  = NULL; /* S(k)=Σ_{d|k}σ(d) */
static idx_t  arrays_cap   = 0;                /* allocated size of arrays */

/* Ensure σ and S arrays are ready up to at least n. */
static void ensure_arrays(idx_t n) {
    if (n + 1 <= arrays_cap) return;

    size_t new_cap = (size_t)(n + 1);

    unsigned long long *new_sigma =
        (unsigned long long *)realloc(sigma_arr, new_cap * sizeof(unsigned long long));
    unsigned long long *new_sum =
        (unsigned long long *)realloc(sum_sigma, new_cap * sizeof(unsigned long long));

    if (!new_sigma || !new_sum) abort();

    sigma_arr  = new_sigma;
    sum_sigma  = new_sum;

    memset(sigma_arr, 0, new_cap * sizeof(unsigned long long));
    memset(sum_sigma, 0, new_cap * sizeof(unsigned long long));

    /* Compute σ(k). */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            sigma_arr[j] += i;

    /* Compute S(k) = Σ_{d|k} σ(d). */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            sum_sigma[j] += sigma_arr[i];

    arrays_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1; /* a(0)=1 */

    /* Prepare divisor data. */
    ensure_arrays(n);

    /* Grow the cache if needed. */
    if ((idx_t)(n + 1) > a_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache =
            (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        a_cache = new_cache;

        /* Initialize new cache slots to zero. */
        for (idx_t i = a_cap; i < new_cap; ++i)
            a_cache[i] = (seq_t)0;
    }

    /* Compute values that are not yet cached. */
    for (idx_t m = a_cap; m <= n; ++m) {
        if (m == 0) {
            a_cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t w   = (seq_t)1; /* (m-1)!/(m-k)! for current k */

        for (idx_t k = 1; k <= m; ++k) {
            idx_t rem = m - k;

            /* Compute λ_k. */
            unsigned long long lambda;
            if (k & 1ULL)   /* odd k */
                lambda = 2ULL * sum_sigma[k];
            else            /* even k */
                lambda = 2ULL * (sum_sigma[k] - sum_sigma[k / 2]);

            /* term = a[rem] * w * λ_k */
            seq_t term = seq_mul(a_cache[rem], w);
            if (lambda != 1ULL)
                term = mul_small(term, (unsigned int)lambda);

            sum += term;

            if (k < m)
                w = mul_small(w, (unsigned int)(m - k));
        }

        a_cache[m] = sum;
    }

    a_cap = (idx_t)(n + 1);
    return a_cache[n];
}
