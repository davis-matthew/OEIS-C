/*
 * A298160  G.f.: Product_{k>=1} 1/(1+prime(k)*x^prime(k)).
 * https://oeis.org/A298160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1 + prime(k)·x^prime(k))
 * Range:     i8 n <= 14, i16 n <= 26, i32 n <= 56, i64 n <= 116,
 *            i128 n <= 237, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* = coefficient of x^n in   ∏_{p prime} ∑_{m≥0} (-1)^m p^m x^{m p} */

#include <stddef.h>   /* for size_t, idx_t, seq_t are defined by the build */
#include <stdint.h>   /* for uint8_t, uint32_t */
#include <stdlib.h>   /* for malloc, free, calloc, realloc */

seq_t a(idx_t n)
{
    /* Handle the simple case n = 0 immediately.  */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate the coefficient array a[0..n], zero‑initialized.  */
    seq_t *coeff = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!coeff)
        return (seq_t)0;                /* allocation failed – unlikely in tests */
    coeff[0] = (seq_t)1;                 /* a(0) = 1 */

    /* ------------------------------------------------------------------
     * Build the list of all primes ≤ n using a classic sieve.
     * ------------------------------------------------------------------ */
    size_t limit = (size_t)n;
    uint8_t *is_composite = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!is_composite) {
        free(coeff);
        return (seq_t)0;
    }

    /* Dynamic array for primes.  Initial guess  limit/10 + 10  is safe for n ≤ 10000. */
    size_t primes_cap = limit / 10 + 10;
    uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) {
        free(is_composite);
        free(coeff);
        return (seq_t)0;
    }
    size_t primes_cnt = 0;

    for (size_t p = 2; p <= limit; ++p) {
        if (!is_composite[p]) {
            /* store the prime */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) {
                    free(is_composite);
                    free(coeff);
                    free(primes);
                    return (seq_t)0;
                }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)p;

            /* mark multiples of p */
            if (p * p <= limit) {
                for (size_t mul = p * p; mul <= limit; mul += p)
                    is_composite[mul] = 1;
            }
        }
    }

    /* ------------------------------------------------------------------
     * Dynamic programming: multiply the existing polynomial by each prime
     * factor 1/(1 + p x^p) = ∑_{m≥0} (-p)^m x^{m p}.
     * The recurrence is:  a[j] += (-p) * a[j - p].
     * ------------------------------------------------------------------ */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        seq_t p_val = (seq_t)primes[pi];
        seq_t neg_p = -p_val;
        idx_t p_idx = (idx_t)primes[pi];
        for (idx_t j = p_idx; j <= n; ++j) {
            coeff[j] += neg_p * coeff[j - p_idx];
        }
    }

    /* Retrieve the requested coefficient and release memory. */
    seq_t result = coeff[n];
    free(coeff);
    free(is_composite);
    free(primes);
    return result;
}
