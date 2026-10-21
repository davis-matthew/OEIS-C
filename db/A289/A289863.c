/*
 * A289863  Primes of the form k!14-2, where k!14 is the 14-way factorial
 *          number.
 * https://oeis.org/A289863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 15, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289863: Primes of the form k!14 – 2, where k!14 is the 14‑way multifactorial
   (product of k, k‑14, k‑28, … down to the first positive term).  The function
   returns the n‑th such prime (offset 1).  It lazily generates primes and memoises
   them so that calls may arrive in arbitrary order.  The implementation works
   for any seq_t/idx_t pair provided by the build environment (including
   8/16/32/64/128‑bit unsigned integers and arbitrary‑precision integers).  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"   /* to test 64‑bit candidates */
#include "../utils/is_prime_big.h"    /* to test arbitrary‑precision candidates */
#include "../utils/seq_mul.h"         /* multiplicative operation for seq_t */

/* --- Static state for memoisation ----------------------------------- */
static seq_t *prime_cache = NULL;     /* array of primes found so far */
static size_t prime_cache_cap   = 0;   /* allocated capacity                */
static size_t prime_cache_size  = 0;   /* number of primes stored           */
static idx_t  last_k            = 0;   /* largest k processed so far        */

/* --- Utility to grow the prime cache -------------------------------- */
static void grow_prime_cache(void)
{
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                       /* out of memory */
    prime_cache = tmp;
    prime_cache_cap = new_cap;
}

/* --- Main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not part of the sequence but harmless */

    /* Generate primes until we have at least n of them */
    while (prime_cache_size < (size_t)n) {
        /* Advance to the next k */
        last_k++;

        /* Compute k!14 with step 14, detect overflow for 64‑bit seq_t */
        bool overflow = false;
        seq_t prod;
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            /* 64‑bit or smaller: use 128‑bit intermediate to detect overflow */
            unsigned __int128 p128 = 1;
            idx_t s = last_k;
            while (1) {
                p128 *= (unsigned __int128)s;
                if (s <= 14) break;                 /* finished the chain   */
                if (p128 > UINT64_MAX) { /* overflow will persist for larger k */
                    overflow = true;
                    break;
                }
                s -= 14;
            }
            if (!overflow) prod = (seq_t)p128;
        } else {
            /* Arbitrary precision or 128‑bit: multiplication is safe */
            prod = (seq_t)1;
            idx_t s = last_k;
            while (1) {
                prod = seq_mul(prod, (seq_t)s);
                if (s <= 14) break;
                s -= 14;
            }
        }

        if (overflow) {
            /* Further k will produce even larger products that also overflow.
               The 64‑bit build will only be queried for inputs that fit,
               so we can stop here. */
            break;
        }

        /* Candidate prime: k!14 – 2 */
        seq_t candidate = prod - (seq_t)2;

        /* Test primality */
        bool candidate_prime;
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            candidate_prime = is_prime_small((uint64_t)candidate);
        } else {
            candidate_prime = is_prime_big(candidate);
        }

        if (candidate_prime) {
            /* Store the prime */
            if (prime_cache_size == prime_cache_cap)
                grow_prime_cache();
            prime_cache[prime_cache_size++] = candidate;
        }
    }

    /* Return the requested term if available */
    if (prime_cache_size >= (size_t)n)
        return prime_cache[n - 1];

    /* Not enough primes could be generated (overflow on 64‑bit builds).
       The caller should not request such n; return 0 as a safe default. */
    return 0;
}
