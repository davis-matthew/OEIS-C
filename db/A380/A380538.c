/*
 * A380538  Numbers k such that k is not a multiple of A351566(k), where
 *          A351566 is the radix prime of the second least significant nonzero
 *          digit in the primorial base expansion of n, or 1 if there is no
 *          such digit.
 * https://oeis.org/A380538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 165, u16 n <= 9537, u32 n <= 9537, u64 n <= 9537,
 *            u128 n <= 9537, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = k where k is NOT a multiple of A351566(k), 
   where A351566(k) is the radix prime of the second least‑significant non‑zero
   digit in the primorial‑base expansion of k (or 1 if none). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"   /* provides build_primes */

/* Global prime table used for primorial‑base digits */
static idx_t   *prime_list = NULL;
static size_t  prime_cnt   = 0;
static const idx_t PRIME_LIMIT = 200000;  /* arbitrary upper bound for primes */

/* Build the prime list once */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;
    build_primes(PRIME_LIMIT, &prime_list, &prime_cnt);
}

/* Radix prime of k: second least‑significant non‑zero digit in primorial‑base
   representation. Returns 1 if such a digit does not exist. */
static seq_t radix_prime(seq_t k)
{
    seq_t temp = k;
    bool first_seen = false;
    idx_t  second_prime = 1;

    for (size_t i = 0; i < prime_cnt && temp != 0; ++i) {
        idx_t p = prime_list[i];
        seq_t digit = temp % (seq_t)p;
        temp = temp / (seq_t)p;
        if (digit != 0) {
            if (!first_seen) {
                first_seen = true;
            } else {
                second_prime = p;
                break;   /* found the required prime */
            }
        }
    }
    return (seq_t)second_prime;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    static seq_t *terms   = NULL;   /* cached terms of the sequence */
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static seq_t curr_k   = 1;      /* next integer to test */

    init_primes();

    /* Produce terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        seq_t rad = radix_prime(curr_k);
        if (rad > 1 && (curr_k % rad) != 0) {
            /* Ensure capacity */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                terms_cap = new_cap;
            }
            terms[terms_len++] = curr_k;
        }
        ++curr_k;
    }

    return terms[(size_t)n - 1];
}
