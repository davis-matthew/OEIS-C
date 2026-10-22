/*
 * A327798  Expansion of 1 / (1 - Sum_{i>=1, j>=1} x^(i*(j + 1))).
 * https://oeis.org/A327798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 42, u64 n <= 82, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A327798: a(0)=1; a(n)=Σ_{k=1..n} (τ(k)-1)*a(n-k)
   where τ(k) is the number of positive divisors of k. */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for computed sequence values */
    static seq_t *a_cache = NULL;
    static size_t a_cap   = 0;   /* allocated length of a_cache   */
    /* Static cache for divisor counts τ(k) */
    static idx_t *tau = NULL;
    static size_t tau_cap = 0;   /* allocated length of tau array */

    /* Ensure cache is large enough for n+1 elements */
    if (a_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;
        if (a_cache) {
            memcpy(new_cache, a_cache, a_cap * sizeof(seq_t));
            free(a_cache);
        }
        a_cache = new_cache;
        a_cap   = new_cap;
    }

    /* Ensure τ array is large enough */
    if (tau_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        idx_t *new_tau = (idx_t *)malloc(new_cap * sizeof(idx_t));
        if (!new_tau) return (seq_t)0;
        if (tau) {
            memcpy(new_tau, tau, tau_cap * sizeof(idx_t));
            free(tau);
        }
        tau = new_tau;
        tau_cap = new_cap;
    }

    /* Compute τ(k) for k=1..n using a simple divisor sieve */
    for (idx_t i = 1; i <= (idx_t)n; ++i) tau[i] = 0;
    for (idx_t d = 1; d <= (idx_t)n; ++d) {
        for (idx_t m = d; m <= (idx_t)n; m += d) {
            ++tau[m];
        }
    }

    /* Build sequence values up to n */
    if (a_cache[0] != 1) {        /* initialization guard */
        a_cache[0] = (seq_t)1;
    }
    for (idx_t i = 1; i <= (idx_t)n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t coeff = (seq_t)(tau[k] - 1);
            if (coeff == 0) continue;
            sum += coeff * a_cache[i - k];
        }
        a_cache[i] = sum;
    }

    return a_cache[n];
}
