/*
 * A325653  a(n) = the product of numbers k such that sigma(k) = sigma(n).
 * https://oeis.org/A325653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π { k : σ(k) = σ(n) } (product of all k with equal
 *            sum‑of‑divisors)
 * Range:     u8 none, u16 n <= 2, u32 n <= 59, u64 n <= 119, u128 n <= 263,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation precomputes σ(k) for all k up to a fixed limit
    (chosen to cover σ(n) for n up to 10 000).  For each σ value it keeps
    the product of all numbers having that σ.  Subsequent calls simply
    look up the precomputed product. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_mul.h"            /* seq_mul(seq_t, seq_t) */

static seq_t *sigma_global = NULL;      /* σ(k) for k in [0, LIMIT] */
static seq_t *prod_global  = NULL;      /* product for each σ value          */
static size_t prod_size    = 0;
static int initialized     = 0;

static void precompute(void) {
    const idx_t LIMIT = 200000;           /* enough for n ≤ 10 000       */
    /* Allocate σ array */
    sigma_global = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= LIMIT; ++i) sigma_global[i] = 0;

    /* Compute σ(k) by a sieve‑like method */
    for (idx_t i = 1; i <= LIMIT; ++i) {
        seq_t si = (seq_t)i;
        for (idx_t j = i; j <= LIMIT; j += i) {
            sigma_global[j] += si;
        }
    }

    /* Find maximum σ value in this range to size product array */
    seq_t max_sigma = 0;
    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (sigma_global[i] > max_sigma) max_sigma = sigma_global[i];
    }

    prod_global = (seq_t *)malloc((size_t)(max_sigma + 1) * sizeof(seq_t));
    prod_size = (size_t)(max_sigma + 1);
    for (size_t i = 0; i < prod_size; ++i) prod_global[i] = 1;

    /* For each k, multiply it into the product for its σ value */
    for (idx_t k = 1; k <= LIMIT; ++k) {
        seq_t S = sigma_global[k];
        prod_global[(size_t)S] = seq_mul(prod_global[(size_t)S], (seq_t)k);
    }

    initialized = 1;
}

seq_t a(idx_t n) {
    if (!initialized) precompute();
    /* By definition we assume n ≥ 1 and is within precomputed range */
    seq_t S = sigma_global[n];
    return prod_global[(size_t)S];
}
