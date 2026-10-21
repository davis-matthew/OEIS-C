/*
 * A267478  Primes which are squares (mod 55).
 * https://oeis.org/A267478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   ok_residue
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of primes that are squares modulo 55.
   Includes 5 and 11, and any prime p such that p mod 55 is
   in {1,4,9,14,16,26,31,34,36,49}.  The function searches
   primes on demand, storing them for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ok_residue.h"

/* Static storage for all primes found so far and for the
   qualifying primes of the sequence.  The arrays grow
   dynamically as needed. */
static uint64_t *prime_list = NULL;
static size_t prime_len   = 0;
static size_t prime_cap   = 0;

static uint64_t *qual_list  = NULL;
static size_t   qual_len    = 0;
static size_t   qual_cap    = 0;

/* Next odd candidate to test for primality.  Starts at 3. */
static uint64_t next_candidate = 3;
static bool initialized = false;

/* Grow a uint64_t array to accommodate at least one more element. */
static void grow_array(uint64_t **arr, size_t *len, size_t *cap) {
    if (*len >= *cap) {
        size_t new_cap = (*cap ? *cap * 2 : 16);
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);            /* abort on allocation failure     */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* below offset – undefined behaviour */

    /* Lazy initialization of the prime and qualifying arrays. */
    if (!initialized) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) exit(1);
        prime_list[0] = 2;
        prime_len = 1;

        qual_cap = 16;
        qual_list = (uint64_t *)malloc(qual_cap * sizeof(uint64_t));
        if (!qual_list) exit(1);

        initialized = true;
    }

    /* Generate primes until we have enough qualifying primes. */
    while (qual_len < (size_t)n) {
        uint64_t cand = next_candidate;
        bool is_prime = true;

        /* Trial division using previously found primes. */
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t d = prime_list[i];
            if (d * d > cand) break;
            if (cand % d == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store in the prime list. */
            grow_array(&prime_list, &prime_len, &prime_cap);
            prime_list[prime_len++] = cand;

            /* Test the sequence condition. */
            if (cand == 5 || cand == 11 || ok_residue(cand % 55)) {
                grow_array(&qual_list, &qual_len, &qual_cap);
                qual_list[qual_len++] = cand;
            }
        }

        /* Advance to the next odd number. */
        next_candidate += 2;
    }

    /* Return the requested term, cast to seq_t. */
    return (seq_t)qual_list[(size_t)n - 1];
}
