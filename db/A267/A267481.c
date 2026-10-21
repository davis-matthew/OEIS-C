/*
 * A267481  Primes which are squares (mod 31).
 * https://oeis.org/A267481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267481: primes that are quadratic residues modulo 31.
   The sequence is defined by a(1) = 2, a(2) = 5, a(3) = 7, a(4) = 19,
   a(5) = 31, a(6) = 41, a(7) = 47, a(8) = 59, a(9) = 67, a(10) = 71,
   a(11) = 97, a(12) = 101, a(13) = 103, a(14) = 107, a(15) = 109,
   a(16) = 113, a(17) = 131, a(18) = 149, a(19) = 157, a(20) = 163,
   a(21) = 173, a(22) = 191, a(23) = 193, a(24) = 211, ... */

/* The function generates primes on demand by a simple trial‑division
   method, stores those that satisfy the quadratic‑residue test
   modulo 31, and returns the (n‑th) qualifying prime.  The result is
   memoised, so repeated calls with arbitrary or decreasing n are
   handled efficiently. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Quadratic residues modulo 31. 1 at indices that are residues. */
static const bool good_mod[31] = {
    /* 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 */
    1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0
};

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Static, growable storage for all primes found so far. */
    static uint64_t *all_primes  = NULL;
    static size_t   all_count   = 0;
    static size_t   all_cap     = 0;

    /* Static, growable storage for primes that are quadratic residues
       modulo 31. */
    static uint64_t *good_primes = NULL;
    static size_t   good_count  = 0;
    static size_t   good_cap    = 0;

    /* Next integer to test for primeness; start at 2. */
    static uint64_t candidate = 2;

    /* Allocate initial capacity when first called. */
    if (!all_primes) {
        all_cap   = 64;
        all_primes = (uint64_t *)malloc(all_cap * sizeof(uint64_t));
    }
    if (!good_primes) {
        good_cap  = 64;
        good_primes = (uint64_t *)malloc(good_cap * sizeof(uint64_t));
    }

    /* Generate primes until we have at least n qualifying primes. */
    while (good_count < (size_t)n) {
        /* Trial‑division primality test using the primes already found. */
        bool is_prime = true;
        for (size_t i = 0; i < all_count; ++i) {
            uint64_t p = all_primes[i];
            if (p * p > candidate) {
                break;
            }
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime. */
            if (all_count == all_cap) {
                all_cap <<= 1;
                all_primes = (uint64_t *)realloc(all_primes, all_cap * sizeof(uint64_t));
            }
            all_primes[all_count++] = candidate;

            /* Check if candidate is a quadratic residue modulo 31. */
            if (good_mod[(int)(candidate % 31)]) {
                if (good_count == good_cap) {
                    good_cap <<= 1;
                    good_primes = (uint64_t *)realloc(good_primes, good_cap * sizeof(uint64_t));
                }
                good_primes[good_count++] = candidate;
            }
        }

        /* Advance to the next candidate:
           after 2 go to 3, thereafter only odd numbers. */
        if (candidate == 2) {
            candidate = 3;
        } else {
            candidate += 2;
        }
    }

    /* Return the (n‑th) qualifying prime, cast to seq_t. */
    return (seq_t)good_primes[n - 1];
}
