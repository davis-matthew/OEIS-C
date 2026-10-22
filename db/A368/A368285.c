/*
 * A368285  Expansion of e.g.f. exp(2*x) / (1 + 2*log(1 - x)).
 * https://oeis.org/A368285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + 2 * Σ_{k=1..n} (k-1)! * binomial(n,k) * a(n-k) a(0)
 *            = 1 The implementation uses memoisation for a(n) and factorials.
 *            It works for integer seq_t types as well as C++
 *            arbitrary-precision integer classes. The code is valid in C11
 *            and C++17
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../utils/binomial.h"

/* Forward declaration of binomial; assumed available as a helper.
   If not provided, a simple multiplicative implementation is used below. */
#ifndef binomial

#endif

/* Cache for a(n) values */
static seq_t *a_cache = NULL;
static size_t a_cap  = 0;
static idx_t a_max   = 0;

/* Cache for factorials: fact_cache[m] = m!  (fact_cache[0] = 1) */
static seq_t *fact_cache = NULL;
static size_t fact_cap  = 0;
static idx_t fact_max   = 0;

/* Cache for powers of two: pow2_val = 2^pow2_max */
static seq_t pow2_val = 1;
static idx_t pow2_max = 0;

/* Ensure a_cache has size at least n+1 */
static void ensure_a(size_t n)
{
    if (n < a_cap) return;
    size_t new_cap = n + 1;
    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    a_cache = tmp;
    a_cap = new_cap;
}

/* Ensure fact_cache has size at least n+1 */
static void ensure_fact(size_t n)
{
    if (n < fact_cap) return;
    size_t new_cap = n + 1;
    seq_t *tmp = (seq_t*)realloc(fact_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    fact_cache = tmp;
    fact_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* One‑time initialization */
    if (a_cache == NULL) {
        ensure_a(0);
        a_cache[0] = 1;
        a_max = 0;

        ensure_fact(0);
        fact_cache[0] = 1;
        fact_max = 0;

        pow2_val = 1;
        pow2_max = 0;
    }

    /* Extend factorial cache if needed */
    if (n > fact_max) {
        ensure_fact((size_t)n);
        for (idx_t i = fact_max + 1; i <= n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_max = n;
    }

    /* Extend a_cache and compute missing terms */
    if (n > a_max) {
        ensure_a((size_t)n);
        /* Ensure pow2_val is up to 2^a_max */
        while (pow2_max < a_max) {
            pow2_val *= (seq_t)2;
            ++pow2_max;
        }

        for (idx_t m = a_max + 1; m <= n; ++m) {
            /* Update power of two: 2^m */
            pow2_val *= (seq_t)2;
            ++pow2_max;

            seq_t sum = 0;
            /* Compute Σ_{k=1..m} (k-1)! * binomial(m,k) * a(m-k) */
            for (idx_t k = 1; k <= m; ++k) {
                seq_t term = a_cache[m - k];
                term *= binomial(m, k);
                term *= fact_cache[k - 1];   /* (k-1)! */
                sum += term;
            }
            a_cache[m] = pow2_val + (sum * 2);
        }
        a_max = n;
    }

    return a_cache[n];
}
