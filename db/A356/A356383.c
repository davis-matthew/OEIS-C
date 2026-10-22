/*
 * A356383  Primes p such that the sum of p and the next four primes congruent
 *          mod 10 to p is 5 times a prime.
 * https://oeis.org/A356383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 767, u32 n <= 806, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find primes p such that p + the next four primes congruent to p (mod 10)
   equals 5 times a prime.  The function builds the sequence on demand,
   memoizing the terms so repeated calls are fast. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start) */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n) */

static seq_t *terms = NULL;          /* memoized sequence terms */
static size_t terms_cap = 0;          /* capacity of the terms array */
static size_t terms_len = 0;          /* number of stored terms */
static uint64_t nextprime_start = 0; /* start for the next prime search */

/* Return the n‑th term (1‑based) of A356383 */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety; real offset is 1 */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* Next prime after the previous one */
        uint64_t p = next_prime(nextprime_start);
        nextprime_start = p;

        /* Small primes < 7 cannot have four distinct successors with the
           required residue, so skip them */
        if (p < 7) continue;

        uint64_t q = p;         /* current prime candidate */
        uint8_t target_mod = (uint8_t)(p % 10);
        seq_t sum = (seq_t)p;   /* start the sum with p itself */

        /* Find the next four primes congruent to p (mod 10) and add them
           to the sum.  next_prime always returns the next prime > start,
           so we can loop until the modulo matches. */
        for (int i = 0; i < 4; ++i) {
            do {
                q = next_prime(q);
            } while ((q % 10) != target_mod);
            sum += (seq_t)q;
        }

        /* sum is always divisible by 5; check whether sum/5 is prime. */
        seq_t mid = sum / 5; /* division by an integral constant */
        if (!is_prime_big(mid)) continue; /* not a term */

        /* Ensure enough space in the memoization array */
        if (terms_len == terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            terms = tmp;
            terms_cap = new_cap;
        }
        terms[terms_len++] = (seq_t)p;
    }

    return terms[n - 1];
}
