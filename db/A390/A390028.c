/*
 * A390028  Record denominators of (p2-1)/(p1-1), where p1 < p2 are
 *          consecutive primes.
 * https://oeis.org/A390028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: The record denominators of (p₂−1)/(p₁−1), where p₁<p₂ are consecutive primes.
 * The sequence a(n) contains the n‑th value that exceeds all previous denominators.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/*--- static data used by the function ------------------------------------*/

static seq_t *record = NULL;          /* record denominators */
static size_t rec_len  = 0,           /* number of stored records   */
              rec_cap  = 0;           /* capacity of record array   */

static uint64_t *prime = NULL;        /* listed primes for trial division */
static size_t   prime_len  = 0,
                prime_cap  = 0;

static uint64_t candidate   = 2;      /* next candidate for primality test */
static uint64_t last_prime  = 0;      /* last prime used in a pair        */

static seq_t current_max = 0;         /* largest denominator seen so far   */

/*--- helper functions ----------------------------------------------------*/

/* Double the prime array capacity and reallocate it. */
static void grow_prime_array(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint64_t *tmp = (uint64_t *)realloc(prime, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime = tmp;
    prime_cap = new_cap;
}

/* Append a prime to the prime array. */
static void add_prime(uint64_t p)
{
    if (prime_len == prime_cap) grow_prime_array();
    prime[prime_len++] = p;
}

/* Return the next prime after the current candidate.  Uses simple trial division. */
static uint64_t next_prime(void)
{
    while (1) {
        uint64_t p = candidate;
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t pr = prime[i];
            if (pr * pr > p) break;
            if (p % pr == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            add_prime(p);
            /* advance candidate: skip evens after 2 */
            candidate = (p == 2 ? 3 : p + 2);
            return p;
        }
        candidate = (p == 2 ? 3 : p + 2);
    }
}

/*--- sequence function ---------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    /* Ensure we have enough records */
    while (rec_len < (size_t)n) {
        /* Acquire the next prime */
        uint64_t p = next_prime();

        /* If we haven't yet had a previous prime, store it and continue */
        if (last_prime == 0) {
            last_prime = p;
            continue;
        }

        /* Compute the denominator of (p−1)/(last_prime−1) in lowest terms */
        seq_t g = gcd((seq_t)(last_prime - 1), (seq_t)(p - 1));
        seq_t denom = (seq_t)(last_prime - 1) / g;

        /* If a new record is found, store it */
        if (denom > current_max) {
            if (rec_len == rec_cap) {
                size_t new_cap = rec_cap ? rec_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(record, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                record = tmp;
                rec_cap = new_cap;
            }
            record[rec_len++] = denom;
            current_max = denom;
        }

        /* Advance to the next pair */
        last_prime = p;
    }

    return record[(size_t)n - 1];
}
