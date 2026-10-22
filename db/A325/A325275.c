/*
 * A325275  Heinz number of the omega-sequence of n!.
 * https://oeis.org/A325275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   build_primes_upto, get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Heinz number of the omega‑sequence of n! .
   Starting from n! a list of exponents is built; at each step
   Omega is taken, prime(Omega) is multiplied into the answer,
   and the exponents are replaced by their frequency counts.
   The process stops when Omega == 1. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/seq_mul.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* 0! and 1! give empty partitions → result 1 */
    if (n < 2) return (seq_t)1;

    /* Cache primes up to the largest n encountered so far. */
    static uint64_t *primes_cache = NULL;
    static size_t primes_cache_count = 0;
    static idx_t primes_cache_limit = 0;

    if (n > primes_cache_limit) {
        if (primes_cache != NULL) free(primes_cache);
        build_primes_upto(n, &primes_cache, &primes_cache_count);
        primes_cache_limit = n;
    }

    /* Compute initial exponent vector of n! (Legendre's formula). */
    size_t pc = primes_cache_count;
    idx_t *exp_vec = (idx_t *)malloc(pc * sizeof(idx_t));
    for (size_t i = 0; i < pc; ++i) {
        uint64_t p = primes_cache[i];
        idx_t e = 0;
        idx_t div = n / p;
        while (div) {
            e += div;
            div /= p;
        }
        exp_vec[i] = e;
    }

    idx_t *cur = exp_vec;
    size_t cur_sz = pc;
    seq_t prod = (seq_t)1;

    for (;;) {
        /* Compute Omega (sum of exponents). */
        idx_t omega = 0;
        idx_t max_exp = 0;
        for (size_t i = 0; i < cur_sz; ++i) {
            omega += cur[i];
            if (cur[i] > max_exp) max_exp = cur[i];
        }

        /* Multiply by prime(omega). */
        seq_t prime_val = (seq_t)get_prime(omega);
        prod = seq_mul(prod, prime_val);

        /* Termination condition: Omega == 1. */
        if (omega == 1) break;

        /* Build frequency map of current exponents. */
        idx_t *freq = (idx_t *)calloc((size_t)(max_exp + 1), sizeof(idx_t));
        for (size_t i = 0; i < cur_sz; ++i) {
            freq[cur[i]]++;
        }

        /* Build next exponent vector from non‑zero frequencies. */
        idx_t *next = (idx_t *)malloc(cur_sz * sizeof(idx_t)); /* at most cur_sz */
        size_t next_sz = 0;
        for (idx_t e = 1; e <= max_exp; ++e) {
            if (freq[e] != 0) {
                next[next_sz++] = freq[e];
            }
        }

        /* Clean up. */
        free(freq);
        free(cur);

        cur = next;
        cur_sz = next_sz;
    }

    free(cur);
    return prod;
}
