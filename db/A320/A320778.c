/*
 * A320778  Inverse Euler transform of the Euler totient function phi =
 *          A000010.
 * https://oeis.org/A320778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the inverse Euler transform of the Euler totient
 *            function φ(n). * It satisfies: * 1 + Σ_{n≥1} φ(n)x^n = ∏_{k≥1}
 *            (1-x^k)^{-a(k)} , * with a(0)=1. * The following code computes
 *            a(n) for all n ≥ 0 using dynamic memoisation
 * Range:     i8 n <= 10, i16 n <= 23, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache_a = NULL;     /* a[0..max]  */
static seq_t *cache_S = NULL;    /* S[n] = Σ_{d|n} d*a(d) */
static seq_t *cache_phi = NULL;  /* φ[n] for n ≥ 1 */
static idx_t  max_computed = 0;

/* Helper: grow all caches to size new_max (inclusive). */
static void grow_caches(idx_t new_max)
{
    /* Allocate new arrays */
    seq_t *new_a   = (seq_t *)calloc(new_max + 1, sizeof(seq_t));
    seq_t *new_S   = (seq_t *)calloc(new_max + 1, sizeof(seq_t));
    seq_t *new_phi = (seq_t *)calloc(new_max + 1, sizeof(seq_t));

    /* Copy old data */
    if (cache_a)    memcpy(new_a,   cache_a,   (max_computed + 1) * sizeof(seq_t));
    if (cache_S)    memcpy(new_S,   cache_S,   (max_computed + 1) * sizeof(seq_t));
    if (cache_phi)  memcpy(new_phi, cache_phi, (max_computed + 1) * sizeof(seq_t));

    /* Free old arrays */
    free(cache_a);
    free(cache_S);
    free(cache_phi);

    /* Replace with new */
    cache_a   = new_a;
    cache_S   = new_S;
    cache_phi = new_phi;

    /* Initialise φ array (will be overwritten by sieve) */
    for (idx_t i = 0; i <= new_max; ++i) cache_phi[i] = (seq_t)i;

    /* Sieve for Euler's totient */
    for (idx_t i = 2; i <= new_max; ++i) {
        if (cache_phi[i] == (seq_t)i) {          /* i is prime */
            for (idx_t j = i; j <= new_max; j += i) {
                cache_phi[j] -= cache_phi[j] / (seq_t)i;
            }
        }
    }

    /* Ensure a[0] is defined */
    cache_a[0] = 1;
    cache_S[0] = 0;      /* unused but kept for symmetry */

    /* Compute values from old_max+1 up to new_max */
    for (idx_t n = max_computed + 1; n <= new_max; ++n) {
        seq_t phi_n = cache_phi[n];

        /* Compute S[n] = n*φ(n) - Σ_{k=1}^{n-1} S[k]*φ(n-k) */
        seq_t sum_k = 0;
        for (idx_t k = 1; k < n; ++k) {
            sum_k += cache_S[k] * cache_phi[n - k];
        }
        cache_S[n] = (seq_t)n * phi_n - sum_k;

        /* Compute a[n] from S[n] and earlier a[ ] */
        seq_t sum_div = 0;
        for (idx_t d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                idx_t d1 = d;
                idx_t d2 = n / d;

                if (d1 < n) {
                    sum_div += (seq_t)d1 * cache_a[d1];
                }
                if (d2 != d1 && d2 < n) {
                    sum_div += (seq_t)d2 * cache_a[d2];
                }
            }
        }
        cache_a[n] = (cache_S[n] - sum_div) / (seq_t)n;
    }

    max_computed = new_max;
}

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 by definition */

    if (n > max_computed) {
        grow_caches(n);
    }
    return cache_a[n];
}
