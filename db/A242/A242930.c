/*
 * A242930  Primes of the form (k^2+7)/11.
 * https://oeis.org/A242930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 57, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A242930 – the nth prime p such that 11*p–7 is a perfect square.
   The sequence is defined by p = (k²+7)/11 where k ≡ 2 or 9 (mod 11). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"  // provides: bool is_prime_big(seq_t n);

seq_t a(idx_t n) {
    /* Static cache of computed primes */
    static seq_t *primes = NULL;   /* array of primes */
    static size_t  primes_cap = 0; /* allocated capacity */
    static size_t  primes_cnt = 0; /* number of primes stored */
    static idx_t   last_k = 0;     /* largest k processed */

    /* Ensure cache is large enough to hold at least n primes */
    if (n > primes_cnt) {
        /* Allocate or grow cache */
        if (primes == NULL) {
            primes_cap = 128;
            primes = (seq_t*)malloc(primes_cap * sizeof(seq_t));
        }
        while (primes_cnt < n) {
            /* Move to next k */
            last_k++;  /* start from 1 */
            /* Compute (k² + 7)/11 */
            seq_t k_sq = (seq_t)last_k * (seq_t)last_k;
            seq_t v = k_sq + 7;
            if (v % 11 != 0) continue;          /* skip non‑integral candidates */
            seq_t p = v / 11;
            if (!is_prime_big(p)) continue;     /* must be prime */
            /* Store prime p */
            if (primes_cnt >= primes_cap) {
                primes_cap *= 2;
                primes = (seq_t*)realloc(primes, primes_cap * sizeof(seq_t));
            }
            primes[primes_cnt++] = p;
        }
    }
    return primes[n - 1]; /* seq_t is 0‑based in C, but a(1) is the first term */
}
