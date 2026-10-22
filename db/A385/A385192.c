/*
 * A385192  Primes p such that multiplicative order of 5 modulo p is odd.
 * https://oeis.org/A385192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1619, u32 n <= 1833, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A385192: primes p for which ord_p(5) is odd.
 * The routine terminates at the first n‑th such prime; it keeps a
 * light cache so later calls are fast.  It is fully compilable in
 * C11 and C++17.  The code uses only integer arithmetic on 64‑bit
 * unsigned values; the result is cast to `seq_t` on return. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t, uint64_t, seq_t); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t); */

/* ---------- static cache ------------------------------------------- */

static uint64_t *primes_all        = NULL;   /* all primes found so far    */
static size_t   primes_all_len    = 0;       /* number of stored primes    */
static size_t   primes_all_cap    = 0;       /* capacity of primes_all     */

static uint64_t *primes_good       = NULL;   /* primes with odd order of 5 */
static size_t   primes_good_len   = 0;
static size_t   primes_good_cap   = 0;

static uint64_t next_candidate      = 3;     /* smallest candidate after 2 */

/* ---------- helper: ensure cache contains at least n good primes --- */
static void ensure_cache(idx_t n)
{
    /* if we already have enough, nothing to do */
    if (primes_good_len >= (size_t)n)
        return;

    /* initialise caches if they are empty */
    if (primes_all_len == 0) {
        if (primes_all_cap == 0) {
            primes_all_cap = 4;
            primes_all     = (uint64_t *)malloc(primes_all_cap * sizeof(uint64_t));
        }
        primes_all[primes_all_len++] = 2;
    }
    if (primes_good_len == 0) {
        if (primes_good_cap == 0) {
            primes_good_cap = 4;
            primes_good     = (uint64_t *)malloc(primes_good_cap * sizeof(uint64_t));
        }
        primes_good[primes_good_len++] = 2;
    }

    /* keep searching until we have enough good primes */
    while (primes_good_len < (size_t)n) {
        /* test primeness of the candidate */
        if (is_prime_big((seq_t)next_candidate)) {
            /* store in the list of all primes */
            if (primes_all_len == primes_all_cap) {
                primes_all_cap = primes_all_cap ? primes_all_cap * 2 : 4;
                primes_all     = (uint64_t *)realloc(primes_all,
                                                     primes_all_cap * sizeof(uint64_t));
            }
            primes_all[primes_all_len++] = next_candidate;

            /* compute the multiplicative order of 5 modulo p */
            const uint64_t p     = next_candidate;
            uint64_t        order = p - 1;          /* order divides p-1 */
            uint64_t        tmp   = p - 1;          /* remaining factor */

            for (size_t i = 0;
                 i < primes_all_len && primes_all[i] * primes_all[i] <= tmp;
                 ++i)
            {
                const uint64_t q = primes_all[i];
                if (tmp % q == 0) {
                    while (tmp % q == 0) tmp /= q;
                    while (order % q == 0 &&
                           mod_pow((seq_t)5, order / q, (seq_t)p) == (seq_t)1)
                        order /= q;
                }
            }
            if (tmp > 1) { /* a remaining prime factor > sqrt(p-1) */
                const uint64_t q = tmp;
                while (order % q == 0 &&
                       mod_pow((seq_t)5, order / q, (seq_t)p) == (seq_t)1)
                    order /= q;
            }

            /* if the order is odd, remember this prime */
            if ((order & 1U) == 1U) {
                if (primes_good_len == primes_good_cap) {
                    primes_good_cap = primes_good_cap ? primes_good_cap * 2 : 4;
                    primes_good     = (uint64_t *)realloc(primes_good,
                                                          primes_good_cap * sizeof(uint64_t));
                }
                primes_good[primes_good_len++] = p;
            }
        }
        next_candidate += 2;  /* skip even numbers */
    }
}

/* ------------------------------------------------------------------- */
/* Sequence function required by the framework */

seq_t a(idx_t n)
{
    /* Arguments are guaranteed n >= 1 */
    ensure_cache(n);
    return (seq_t)primes_good[(size_t)(n - 1)];
}
