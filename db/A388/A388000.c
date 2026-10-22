/*
 * A388000  Numbers k such that phi(k) = pi(k) + tau(k).
 * https://oeis.org/A388000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, calc_phi, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that φ(k) = π(k) + τ(k). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/calc_phi.h"          /* seq_t calc_phi(seq_t m); */
#include "../utils/compute_pi.h"        /* seq_t compute_pi(idx_t n); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt); */
#include "../utils/_floor_isqrt_u64.h"  /* idx_t _floor_isqrt_u64(idx_t n); */

/* --------------------------------------------------------------------- */
/* Cached sequence values.                                               */
static seq_t *found_terms = NULL;   /* array of found k's                    */
static size_t found_len   = 0;      /* how many we have stored              */
static size_t found_cap   = 0;      /* current capacity of the array         */

/* --------------------------------------------------------------------- */
/* Prime table used for phi, pi and tau computations.                 */
static uint64_t *primes      = NULL; /* list of primes                      */
static size_t   prime_cnt    = 0;    /* number of primes in the list         */
static uint64_t prime_limit  = 0;    /* maximum prime value in the list       */

static uint64_t last_checked = 0; /* last integer examined when searching */

/* --------------------------------------------------------------------- */
/* Extend the prime list so that all primes <= limit are present. */
static void extend_primes(uint64_t limit)
{
    if (limit <= prime_limit)
        return;
    uint64_t *new_list = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_list, &new_cnt);
    free(primes);
    primes = new_list;
    prime_cnt = new_cnt;
    prime_limit = limit;
}

/* --------------------------------------------------------------------- */
/* τ(n) – divisor function, counting the number of positive divisors.   */
static seq_t tau_seq(uint64_t n)
{
    seq_t result = 1;
    uint64_t d = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > d)
            break;
        if (d % p == 0) {
            unsigned exp = 0;
            while (d % p == 0) {
                d /= p;
                ++exp;
            }
            result = result * ((seq_t)exp + 1);
        }
    }
    if (d > 1)
        result = result * 2;
    return result;
}

/* --------------------------------------------------------------------- */
/* Main sequence function.  It lazily generates terms until it has the
   requested n‑th value.  The function works for any order of calls.      */
seq_t a(idx_t n)
{
    if (n == 0)                 /* safety for out‑of‑bounds calls */
        return (seq_t)0;

    /* Allocate storage for found terms on first use. */
    if (found_cap == 0) {
        found_cap = 16;
        found_terms = (seq_t*)calloc(found_cap, sizeof(seq_t));
    }

    /* Already have the answer? */
    if (found_len >= (size_t)n)
        return found_terms[n - 1];

    /* Ensure we can compute φ and π for the upcoming candidates. */
    if (prime_limit < 2)
        extend_primes(2);

    /* Search forward from the last checked integer. */
    uint64_t candidate = last_checked + 1;
    while (found_len < (size_t)n) {
        uint64_t sqrt_c = _floor_isqrt_u64(candidate);
        extend_primes(sqrt_c);

        seq_t phi = calc_phi((seq_t)candidate);
        seq_t pi  = compute_pi(candidate);
        seq_t tau = tau_seq(candidate);

        if (phi == pi + tau) {
            /* Grow the array if necessary. */
            if (found_len == found_cap) {
                found_cap *= 2;
                found_terms = (seq_t*)realloc(found_terms,
                                              found_cap * sizeof(seq_t));
            }
            found_terms[found_len++] = (seq_t)candidate;
        }

        last_checked = candidate;
        ++candidate;
        if (candidate == 0)   /* overflow guard */
            break;
    }

    return found_terms[n - 1];
}
