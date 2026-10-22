/*
 * A302592  One, powers of 2, and prime numbers of prime index.
 * https://oeis.org/A302592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth element of the increasing union of powers of two
 *            and prime numbers whose index in the sequence of primes is also
 *            prime. I.e., a(n) = sort( A000079 ∪ A006450 )
 * Range:     u8 n <= 19, u16 n <= 558, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static state for memoisation */
static seq_t *term_arr   = NULL; /* array of computed terms */
static size_t term_cap   = 0;     /* allocated capacity */
static size_t term_cnt   = 0;     /* number of terms stored */

/* Streams for generating the sequence */
static seq_t  next_power = 1;          /* next power of two (2^k) */
static seq_t  pp_next   = 0;          /* next prime-of-prime candidate, 0 means “need to compute” */
static idx_t  prime_scan = 2;         /* number being tested for primality */
static idx_t  prime_count = 0;        /* so far number of primes found (index of current prime) */

/* Perform a realloc with a cast suitable for both C and C++ */
static void ensure_capacity(size_t needed)
{
    if (needed <= term_cap)
        return;
    size_t new_cap = term_cap ? term_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(term_arr, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1);
    term_arr = new_arr;
    term_cap = new_cap;
}

/* Find the next prime whose index in the prime list is also prime.
   The value is stored in pp_next. */
static void advance_pp_next(void)
{
    /* prime_scan is at the next integer to test (starts at 2)   */
    while (true) {
        /* Skip non‑primes */
        while (!is_prime(prime_scan))
            prime_scan++;

        /* prime_scan is now prime */
        prime_count++;               /* increment the index counter */

        if (is_prime(prime_count)) { /* index is also prime */
            pp_next = prime_scan;    /* this prime is the next prime-of-prime */
            prime_scan++;            /* advance for the next search */
            break;
        }
        prime_scan++;                /* continue to the next integer */
    }
}

/* Generate terms until the array contains at least n terms (1‑based). */
static void generate_up_to(idx_t n)
{
    while (term_cnt < n) {
        /* Ensure we have a candidate from the prime‑of‑prime stream. */
        if (pp_next == 0)
            advance_pp_next();

        seq_t chosen;
        if (next_power < pp_next) {
            chosen = next_power;
            next_power <<= 1;          /* next power of two */
        } else if (next_power > pp_next) {
            chosen = pp_next;
            advance_pp_next();          /* consume the candidate and find the next */
        } else { /* equal – avoid duplication */
            chosen = next_power;
            next_power <<= 1;
            advance_pp_next();          /* advance both streams */
        }

        /* Store the chosen term */
        ensure_capacity(term_cnt + 1);
        term_arr[term_cnt++] = chosen;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;       /* offset is 1; guard against zero */
    generate_up_to(n);
    return term_arr[n - 1];
}
