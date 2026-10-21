/*
 * A269903  Integers n such that A002110(n) / 2 is the sum of 4 but no fewer
 *          nonzero squares.
 * https://oeis.org/A269903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269903: a(n) = the nth integer k such that the product of the
 * first k‑1 odd primes is congruent to 7 modulo 8. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ---------- Prime generation ------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t     prime_cnt  = 0;
static size_t     prime_cap  = 0;

/* Add a prime to the list. */
static void add_prime(uint64_t p)
{
    if (prime_cap == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_cnt >= prime_cap) {
        prime_cap *= 2;
        prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }
    prime_list[prime_cnt++] = p;
}

/* Return the next odd prime (starting from 3). */
static uint64_t next_odd_prime(void)
{
    static uint64_t candidate = 3;
    for (;;) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            add_prime(candidate);
            uint64_t result = candidate;
            candidate += 2;   /* next odd candidate */
            return result;
        }
        candidate += 2;
    }
}

/* ---------- Sequence cache ---------------------------------------- */
static seq_t *terms   = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* State for generating new terms. */
static uint8_t prod_mod8 = 1;          /* product of primes modulo 8 */
static size_t primes_processed = 0;    /* number of primes already processed */

/* Ensure the cache holds at least 'need' terms. */
static void ensure_terms(size_t need)
{
    while (terms_len < need) {
        uint64_t p = next_odd_prime();
        prod_mod8 = (uint8_t)((prod_mod8 * (p & 7u)) & 7u);
        ++primes_processed;              /* processed one more prime */
        if (prod_mod8 == 7U) {
            if (terms_cap == 0) {
                terms_cap = 32;
                terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
            } else if (terms_len >= terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)(primes_processed + 1);
        }
    }
}

/* ---------- Public function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for index 0 */
    if (terms_len < (size_t)n) ensure_terms((size_t)n);
    return terms[n - 1];
}
