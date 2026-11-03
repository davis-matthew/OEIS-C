/*
 * A393201  Numbers k that are neither squarefree nor prime powers, such that
 *          k/rad(k) != lpf(k), where rad = A007947 and lpf = A020639.
 * https://oeis.org/A393201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8912 (2026-11-01)
 * Helpers:   is_squarefree, omega, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A393201: Numbers k that are neither squarefree nor prime powers,
   such that k/rad(k) != lpf(k). The function enumerates integers and
   selects those that satisfy the conditions. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"
#include "../utils/omega.h"
#include "../utils/radical.h"

/* Return the smallest prime factor of n   (n > 1) */
static inline idx_t lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;   /* n itself is prime – should not be called for primes */
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Return the nth term (1‑based) of A393201. */
seq_t a(idx_t n)
{
    static idx_t   next_candidate = 2;      /* next integer to test       */
    static idx_t   terms_len      = 0;      /* number of terms stored     */
    static size_t  terms_cap      = 0;      /* allocated capacity         */
    static seq_t * terms          = NULL;   /* array of terms             */

    if (n == 0) return 0; /* index 0 is not used */

    /* Grow the cache as far as required */
    while (terms_len < n) {
        /* Find the next integer that satisfies the definition */
        while (true) {
            /* Skip 1, prime powers or squarefree numbers */
            if (is_squarefree(next_candidate)) {
                ++next_candidate;
                continue;
            }
            int distinct = omega(next_candidate);
            if (distinct <= 1) {          /* only one distinct prime ⇒ prime power */
                ++next_candidate;
                continue;
            }

            seq_t rad_val = radical(next_candidate);
            seq_t ratio   = ((seq_t)next_candidate) / rad_val;
            seq_t lpf_val = (seq_t)lpf(next_candidate);

            if (ratio != lpf_val) {
                /* Store the term */
                if (terms_len >= terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                    terms = seq_realloc(terms, terms_cap, new_cap);
                    terms_cap = new_cap;
                }
                terms[terms_len++] = (seq_t)next_candidate;
                ++next_candidate;
                break; /* proceed to the next term */
            } else {
                ++next_candidate;
            }
        }
    }

    return terms[n - 1];
}
