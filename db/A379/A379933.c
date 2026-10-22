/*
 * A379933  Expansion of e.g.f. 1/( exp(-x) - x )^2.
 * https://oeis.org/A379933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+1)*(k+2)^(n-k)/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"     /* seq_t big_pow(seq_t base, seq_t exp); */

/* Factorial table and a cache for sequence values.
   The tables are expanded lazily to the maximum n requested. */
static seq_t *fact_table = NULL;  /* fact_table[i] = i! */
static idx_t  fact_size = 0;      /* allocated size of fact_table */

static seq_t *a_cache = NULL;     /* cache of a(n) values */
static idx_t  cache_size = 0;     /* allocated size of a_cache */

/* Expand the factorial table up to 'n' (inclusive). */
static void ensure_factorial(idx_t n)
{
    if (n + 1 <= fact_size) return;
    idx_t new_size = n + 1;
    seq_t *new_fact = (seq_t*)realloc(fact_table, new_size * sizeof(seq_t));
    if (!new_fact) { abort(); }          /* allocation failed */
    fact_table = new_fact;
    if (fact_size == 0) {
        fact_table[0] = 1;
        fact_size = 1;
    }
    for (idx_t i = fact_size; i < new_size; ++i) {
        fact_table[i] = fact_table[i - 1] * (seq_t)i;
    }
    fact_size = new_size;
}

/* Expand the cache of a(n) values up to 'n' (inclusive) and compute missing entries. */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t*)realloc(a_cache, new_size * sizeof(seq_t));
    if (!new_cache) { abort(); }
    a_cache = new_cache;

    for (idx_t i = cache_size; i < new_size; ++i) {
        /* For each i, compute a(i) using the factorial ratio formulation */
        seq_t sum = 0;
        seq_t fact_i = fact_table[i];               /* i! */
        for (idx_t k = 0; k <= i; ++k) {
            /* ratio = i! / (i-k)! */
            seq_t ratio = fact_i / fact_table[i - k];
            /* term = ratio * (k+1) * (k+2)^(i-k) */
            seq_t term = ratio;
            term *= (seq_t)(k + 1);
            term *= big_pow((seq_t)(k + 2), (seq_t)(i - k));
            sum += term;
        }
        a_cache[i] = sum;
    }
    cache_size = new_size;
}

/* Public sequence function as required by the harness. */
seq_t a(idx_t n)
{
    /* Ensure factorials up to n have been computed. */
    ensure_factorial(n);
    /* Ensure cache entries up to n are available. */
    ensure_cache(n);
    return a_cache[n];
}
