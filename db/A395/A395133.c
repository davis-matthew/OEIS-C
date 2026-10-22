/*
 * A395133  Number of monotone and order decreasing transformations on an
 *          n-chain.
 * https://oeis.org/A395133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n - 2^(n+1) + Fib(2n+1) + L(n) (L(n) = large Schröder
 *            number)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External helper declarations */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/fib.h"    /* seq_t fib(idx_t n) */
#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp) */

/* Cache for large Schröder numbers L(n) (indices start at 0) */
static seq_t *schroeder_cache = NULL;
static idx_t schroeder_cache_size = 0;   /* number of allocated elements (size) */

/* Compute large Schröder numbers up to at least n */
static void ensure_schroeder_cache(idx_t n)
{
    if (n + 1 <= schroeder_cache_size)
        return;

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(schroeder_cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed; terminate gracefully. */
        exit(1);
    }
    schroeder_cache = new_cache;

    if (schroeder_cache_size == 0) {
        schroeder_cache[0] = (seq_t)1;          /* L(0) */
        if (new_size > 1) {
            schroeder_cache[1] = (seq_t)2;      /* L(1) */
        }
        schroeder_cache_size = 2;
    } else if (schroeder_cache_size == 1) {
        /* Only L(0) cached so far; compute L(1) */
        if (new_size > 1) {
            schroeder_cache[1] = (seq_t)2;
        }
        schroeder_cache_size = 2;
    }

    for (idx_t i = schroeder_cache_size; i <= n; ++i) {
        seq_t sum = schroeder_cache[i - 1];
        for (idx_t k = 0; k <= i - 1; ++k) {
            sum += schroeder_cache[k] * schroeder_cache[i - 1 - k];
        }
        schroeder_cache[i] = sum;
    }
    schroeder_cache_size = n + 1;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 */

    /* Ensure the large Schröder cache contains L(n) */
    ensure_schroeder_cache(n);

    /* Compute the terms of the formula */
    seq_t pow_val      = pow2((idx_t)(n + 1));    /* 2^(n+1) */
    seq_t fib_val      = fib((idx_t)(2 * n + 1)); /* Fibonacci(2n+1) */
    seq_t schroeder_val = schroeder_cache[n];     /* L(n) */

    /* sum = 1 + n + fib_val + schroeder_val */
    seq_t sum = (seq_t)1;
    sum += (seq_t)n;
    sum += fib_val;
    sum += schroeder_val;

    /* a(n) = sum - pow_val */
    seq_t result = sum - pow_val;
    return result;
}
