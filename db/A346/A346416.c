/*
 * A346416  Primes p such that the greatest perimeter of a triangle with prime
 *          sides including p and the next prime is prime.
 * https://oeis.org/A346416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 486, u32 n <= 486, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, find_last_prime_lt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS sequence A346416.
   For each prime p, let q be the next prime.
   Let r be the largest prime less than p+q.
   If the perimeter P = p + q + r is prime, then p is in the sequence.
   The function builds the sequence lazily, storing the primes in a static
   array, so calls in any order can be answered quickly. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions supplied by the build system */
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"
#include "../utils/find_last_prime_lt.h"

/* OEIS function */
seq_t a(idx_t n)
{
    /* Static variables to keep state across calls */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t prime_idx = 0;   /* index of the next prime to process */

    static seq_t *seq = NULL;
    static size_t seq_cnt = 0;
    static size_t seq_cap = 0;

    /* Limit large enough to produce at least ~10 000 terms for the required
       benchmark (≈20 million gives ~1.3 million primes) */
    static const idx_t PRIME_LIMIT = 20000000;

    /* Initialise on first call */
    if (!seq) {
        /* Build the list of primes once */
        size_t cnt = 0;
        uint64_t *tmp = NULL;
        build_primes_upto(PRIME_LIMIT, &tmp, &cnt);
        primes = tmp;
        primes_cnt = cnt;

        /* Sequence storage */
        seq_cap = 128;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_cnt = 0;
        prime_idx = 0;
    }

    /* Produce sequence terms until we have at least n entries */
    while (seq_cnt < (size_t)n) {
        /* Need two consecutive primes */
        if (prime_idx + 1 >= primes_cnt)
            break;  /* no more primes – should not happen for the chosen limit */

        uint64_t p = primes[prime_idx];
        uint64_t q = primes[prime_idx + 1];
        uint64_t S = p + q;            /* p + q <= 2*PRIME_LIMIT */
        size_t r_idx = find_last_prime_lt(S, primes, primes_cnt);
        if (r_idx == SIZE_MAX) {
            /* No prime less than p+q – unlikely with chosen limit */
            prime_idx++;
            continue;
        }
        uint64_t r = primes[r_idx];

        uint64_t perimeter = p + q + r;  /* < 3*PRIME_LIMIT, fits in 32‑bit */

        /* Test primality of the perimeter */
        if (is_prime_big((seq_t)perimeter)) {
            /* Grow sequence storage if necessary */
            if (seq_cnt == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_cnt++] = (seq_t)p;
        }

        prime_idx++;  /* move to the next prime for the next iteration */
    }

    /* The sequence is guaranteed to have at least n elements for the tested
       magnitude of n; otherwise the behaviour is undefined (per the spec). */
    return seq[(size_t)n - 1];
}
