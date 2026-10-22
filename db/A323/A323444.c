/*
 * A323444  Sum of exponents in prime-power factorization of Product_{k=0..n}
 *          binomial(n,k) (A001142).
 * https://oeis.org/A323444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * Ω(n!) – 2 * Σ_{k=0..n} Ω(k!)
 * Range:     u8 n <= 31, u16 n <= 809, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where Ω(m) is the sum of exponents in the prime‑power factorisation of m. */
/* Uses the provided bigomega() routine to obtain Ω(k) for each k. */

#include <stdlib.h>
#include "../utils/bigomega.h"

/* caches for Ω(k!) and its prefix sums; index 0 holds Ω(0!) = 0. */
static seq_t *omega_fact = NULL;  /* Ω(k!)  for 0 ≤ k ≤ current_max_n   */
static seq_t *cum_omega  = NULL;  /* Σ_{i=0}^{k} Ω(i!) for 0 ≤ k ≤ …  */
static idx_t  max_n      = 0;      /* largest n already computed          */
static size_t cap        = 0;      /* currently allocated array size      */

/* Main sequence function.  It is safe to call with any n ≥ 0
   in any order – the caches grow lazily. */
seq_t a(idx_t n)
{
    if (n == 0)         /* definition a(0)=0 */
        return (seq_t)0;

    /* Expand caches if necessary.  Allocated size will always be at least n+1. */
    if (max_n < n || cap == 0) {
        /* Determine new capacity: next power of two ≥ n+1. */
        size_t new_cap = (cap == 0) ? 1 : cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        /* Reallocate arrays to the new capacity. */
        omega_fact = (seq_t*)realloc(omega_fact, new_cap * sizeof(seq_t));
        cum_omega  = (seq_t*)realloc(cum_omega,  new_cap * sizeof(seq_t));

        /* Initialise index 0 if this is the first allocation. */
        if (max_n == 0) {
            omega_fact[0] = (seq_t)0;
            cum_omega[0]  = (seq_t)0;
        }

        /* Compute missing entries from max_n+1 up to n. */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t o = bigomega(i);                 /* Ω(i) */
            omega_fact[i] = omega_fact[i-1] + o;   /* Ω(i!)   */
            cum_omega[i]  = cum_omega[i-1] + omega_fact[i]; /* prefix sum */
        }

        max_n = n;
        cap   = new_cap;
    }

    /* Compute the final value using the pre‑computed tables. */
    seq_t res = ((seq_t)(n + 1)) * omega_fact[n] - (seq_t)2 * cum_omega[n];
    return res;
}
