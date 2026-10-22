/*
 * A377430  Numbers k such that there is exactly one squarefree number between
 *          prime(k)+1 and prime(k+1)-1.
 * https://oeis.org/A377430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 5738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   enlarge_if_needed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A377430: numbers k such that exactly one squarefree number
   lies between prime(k)+1 and prime(k+1)-1.  The algorithm
   precomputes a sieve up to a fixed limit, marks integers that
   are divisible by a square, gathers all primes, then scans each
   interval to count the square‑free values. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/enlarge_if_needed.h"

/* Static storage for the sieve and results. */
static uint8_t *not_prime = NULL;      /* 1 if composite, 0 if prime   */
static uint8_t *bad_square = NULL;     /* 1 if divisible by a square   */
static uint32_t *prime_list = NULL;    /* list of primes found          */
static size_t prime_cnt = 0, prime_cap = 0;
static seq_t *qual_k = NULL;           /* qualifying indices (k values) */
static size_t qual_cnt = 0, qual_cap = 0;
static bool initialized = false;

/* A generous upper bound that covers the first 10 000 terms of the
   sequence with plenty of margin.  It is also small enough that the
   sieve and scanning complete in well under two seconds. */
#define MAX_LIMIT 30000000UL  /* 30 million */

/* Build all data structures once. */
static void init(void) {
    if (initialized) return;
    const size_t limit = MAX_LIMIT;

    /* Allocate the sieve arrays. */
    not_prime   = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    bad_square  = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!not_prime || !bad_square) return;

    /* Sieve of Eratosthenes: mark composites. */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!not_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                not_prime[j] = 1;
        }
    }

    /* Build the prime list. */
    prime_cap = limit / 10;      /* rough upper bound; will grow if needed */
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    for (size_t i = 2; i <= limit; ++i) {
        if (!not_prime[i]) {
            enlarge_if_needed((void **)&prime_list, &prime_cap,
                              prime_cnt + 1, sizeof(uint32_t));
            prime_list[prime_cnt++] = (uint32_t)i;
        }
    }

    /* Mark numbers that are divisible by a square of a prime. */
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint32_t p = prime_list[idx];
        uint64_t p2 = (uint64_t)p * (uint64_t)p;
        if (p2 > limit) break;
        for (uint64_t j = p2; j <= limit; j += p2)
            bad_square[j] = 1;
    }

    /* Find qualifying k-values. */
    qual_cap = 256;
    qual_k = (seq_t *)malloc(qual_cap * sizeof(seq_t));
    for (size_t k = 1; k < prime_cnt; ++k) {          /* k is 1‑based index of primes */
        uint32_t pk  = prime_list[k - 1];
        uint32_t pk1 = prime_list[k];
        size_t count = 0;
        for (uint32_t m = pk + 1; m < pk1; ++m)
            if (!bad_square[m]) ++count;
        if (count == 1) {
            enlarge_if_needed((void **)&qual_k, &qual_cap,
                              qual_cnt + 1, sizeof(seq_t));
            qual_k[qual_cnt++] = (seq_t)k;
        }
    }

    initialized = true;
}

/* Return the nth element of the sequence (1‑based).  If n is out of
   range the function returns 0; callers should not request such a
   value. */
seq_t a(idx_t n) {
    init();
    if (n == 0 || (idx_t)(n - 1) >= qual_cnt)
        return 0;
    return qual_k[n - 1];
}
