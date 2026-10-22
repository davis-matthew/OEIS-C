/*
 * A322842  Primes p such that both p+2 and p-2 are neither prime nor
 *          semiprime.
 * https://oeis.org/A322842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Primes p such that both p+2 and p-2 are neither prime nor
 *            semiprime (i.e. each has at least three prime factors)
 * Range:     u8 n <= 1, u16 n <= 1483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/_floor_isqrt_u64.h"   /* _floor_isqrt_u64(idx_t)                 */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt) */
#include "../utils/isPrimeTrial.h"      /* bool isPrimeTrial(seq_t n)              */

/* --------------------------------------------------------------------- */
/* ----  Dynamic storage for the sequence values                         */
/* --------------------------------------------------------------------- */
static seq_t *results = NULL;
static size_t results_len = 0;
static size_t results_cap = 0;

/* Append a new term, expanding the storage as required. */
static void append_to_results(seq_t val)
{
    if (results_len == results_cap) {
        size_t new_cap = results_cap ? results_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        results = tmp;
        results_cap = new_cap;
    }
    results[results_len++] = val;
}

/* --------------------------------------------------------------------- */
/* ----  Dynamic prime table                                              */
/* --------------------------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* array of all primes <= prime_limit */
static size_t     prime_count = 0;
static idx_t      prime_limit = 0;

/* Extend the prime table so that all primes <= limit are stored. */
static void extend_primes_to(idx_t limit)
{
    if (limit <= prime_limit) return;
    uint64_t *new_list = NULL;
    size_t    new_cnt  = 0;
    build_primes_upto(limit, &new_list, &new_cnt);  /* provided by utils */
    if (prime_list) free(prime_list);
    prime_list = new_list;
    prime_count = new_cnt;
    prime_limit = limit;
}

/* Count the total number of prime factors (with multiplicity) of m.
   Return true iff the count is at least 3.  The function stops
   counting once the third factor is found. */
static bool has_three_or_more(idx_t m)
{
    if (m < 2) return false;
    idx_t cnt = 0;
    idx_t tmp = m;

    /* Ensure we have primes up to sqrt(tmp). */
    idx_t lim = (idx_t)_floor_isqrt_u64(tmp) + 1;
    extend_primes_to(lim);

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        while (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (cnt >= 3) return true;
        }
    }
    if (tmp > 1) {
        ++cnt;
        if (cnt >= 3) return true;
    }
    return cnt >= 3;
}

/* --------------------------------------------------------------------- */
/* ----  Main sequence function                                          */
/* --------------------------------------------------------------------- */

/* The current odd candidate to test.  Starts at 3.                */
static idx_t next_candidate = 3;

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1 */

/* Generate terms until we have at least n of them. */
    while (results_len < n) {
        /* Test whether next_candidate is prime. */
        if (isPrimeTrial((seq_t)next_candidate)) {
            /* Check the condition on p-2 and p+2.  Both must have
               at least three prime factors. */
            if (has_three_or_more(next_candidate - 2) &&
                has_three_or_more(next_candidate + 2)) {
                append_to_results((seq_t)next_candidate);
            }
        }
        /* Advance to the next odd number. */
        next_candidate += 2;
    }

    return results[n - 1];
}
