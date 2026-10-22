/*
 * A372159  E.g.f. A(x) satisfies A(x) = exp( 3 * x * A(x)^(1/3) / (1 - x) ).
 * https://oeis.org/A372159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = exp( 3 * x * A(x)^(1/3) / (1 - x) ) a(n) = 3 * n! *
 *            Σ_{k=1..n} binomial(n-1, k-1) * (k+3)^{k-1} / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   alloc_seq, big_pow, binomial, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* for uint64_t */
#include <stdlib.h>      /* free */

/* Utility functions (provided by the build) */
#include "../utils/big_pow.h"     /* big_pow(seq_t base, seq_t exp)       */
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b)           */
#include "../utils/alloc_seq.h"  /* alloc_seq(size_t)                   */
#include "../utils/free_seq.h"   /* free_seq(seq_t*)                    */

/* Static cache for factorials and sequence values */
static seq_t *fact_cache = NULL;
static size_t  fact_cap  = 0;        /* number of elements in fact_cache   */

static seq_t *seq_cache = NULL;
static size_t  seq_cap  = 0;        /* number of elements in seq_cache    */

/* Ensure factorials up to n inclusive */
static void ensure_factorial(idx_t n)
{
    if ((size_t)n < fact_cap) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)alloc_seq(new_cap);

    if (fact_cache == NULL) {
        new_arr[0] = 1;
        for (idx_t i = 1; i <= n; ++i)
            new_arr[i] = new_arr[i - 1] * (seq_t)i;
    } else {
        size_t old = fact_cap;
        /* copy existing values */
        for (idx_t i = 0; i < old; ++i)
            new_arr[i] = fact_cache[i];
        /* extend */
        for (idx_t i = old; i <= n; ++i)
            new_arr[i] = new_arr[i - 1] * (seq_t)i;
        free_seq(fact_cache);
    }
    fact_cache = new_arr;
    fact_cap   = new_cap;
}

/* Ensure sequence values up to n inclusive */
static void ensure_seq(idx_t n)
{
    if ((size_t)n < seq_cap) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr = (seq_t *)alloc_seq(new_cap);

    size_t old = seq_cap;
    if (seq_cache != NULL) {
        for (idx_t i = 0; i < old; ++i)
            new_arr[i] = seq_cache[i];
        free_seq(seq_cache);
    }
    /* initialise new entries to zero */
    for (idx_t i = old; i <= n; ++i)
        new_arr[i] = 0;

    seq_cache = new_arr;
    seq_cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        ensure_seq(0);
        if (seq_cache[0] == 0)
            seq_cache[0] = 1;
        return seq_cache[0];
    }

    /* precompute needed factorials and array space */
    ensure_factorial(n);
    ensure_seq(n);

    /* already computed? */
    if (seq_cache[n] != 0)
        return seq_cache[n];

    seq_t fact_n = fact_cache[n];
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* comb = binomial(n-1, k-1) */
        seq_t comb = binomial(n - 1, k - 1);

        /* part = n! / k!  (integer) */
        seq_t part = fact_n / fact_cache[k];

        /* pow_val = (k+3)^(k-1) */
        seq_t pow_val = big_pow((seq_t)(k + 3), (seq_t)(k - 1));

        /* term = 3 * comb * pow_val * part */
        seq_t term = comb * pow_val;
        term = term * part;
        term = term * 3;

        sum += term;
    }

    seq_cache[n] = sum;
    return sum;
}
