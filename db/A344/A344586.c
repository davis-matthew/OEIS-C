/*
 * A344586  Numbers k for which A003415(k) >= A001065(k), where A003415 gives
 *          the arithmetic derivative, and A001065 is the sum of proper
 *          divisors.
 * https://oeis.org/A344586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k for which the arithmetic derivative
 *            of k is at least the sum of its proper divisors
 * Range:     u8 n <= 57, u16 n <= 5791, u32 n <= 8875, u64 n <= 8875,
 *            u128 n <= 8875, big any n
 * Helpers:   _floor_isqrt_u64, append_candidate, build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper declarations from the utils library */
#include "../utils/gcd.h"                    /* provides seq_t type */
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/append_candidate.h"

static seq_t *terms = NULL;          /* stored qualifying numbers    */
static size_t  terms_len = 0;        /* how many stored              */
static size_t  terms_cap = 0;        /* current allocated capacity   */

static uint64_t *prime_list = NULL;  /* primes up to prime_limit     */
static size_t   prime_cnt   = 0;     /* number of primes             */
static uint64_t  prime_limit = 0;    /* highest prime precomputed    */

static uint64_t  last_checked = 0;   /* last integer examined        */

/* Ensure that prime_list contains all primes ≤ limit.  */
static void _ensure_primes_up_to(uint64_t limit)
{
    if (limit <= prime_limit)
        return;

    uint64_t *newlist;
    size_t    newcnt;
    build_primes_upto(limit, &newlist, &newcnt);

    free(prime_list);                  /* release old array */
    prime_list = newlist;
    prime_cnt  = newcnt;
    prime_limit = limit;
}

/* Check whether k satisfies the inequality. */
static bool _is_good(uint64_t k)
{
    seq_t n = (seq_t)k;
    seq_t derivative = 0;
    seq_t sigma      = 1;

    uint64_t rem = k;
    uint64_t sqrt_rem = _floor_isqrt_u64(rem);
    _ensure_primes_up_to(sqrt_rem + 1);

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= rem; ++i) {
        uint64_t p = prime_list[i];
        if (rem % p == 0) {
            uint64_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            /* contribution to arithmetic derivative: e * k/p */
            derivative += (seq_t)e * (n / (seq_t)p);

            /* compute term = 1 + p + p^2 + ... + p^e */
            seq_t term = 1;
            seq_t pp   = 1;
            for (uint64_t j = 0; j < e; ++j) {
                pp *= (seq_t)p;
                term += pp;
            }
            sigma *= term;
        }
    }

    if (rem > 1) {  /* remaining prime factor */
        uint64_t p = rem;
        derivative += (seq_t)1 * (n / (seq_t)p);
        sigma *= ((seq_t)1 + (seq_t)p);
    }

    seq_t sumproper = sigma - n;
    return derivative >= sumproper;
}

/* Grow the memoised list until it contains at least n qualifying terms. */
static void _ensure_n_terms(idx_t n)
{
    while (terms_len < n) {
        ++last_checked;
        if (_is_good(last_checked))
            append_candidate(&terms, &terms_len, &terms_cap,
                             (seq_t)last_checked);
    }
}

seq_t a(idx_t n)
{
    _ensure_n_terms(n);
    return terms[n - 1];
}
