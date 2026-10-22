/*
 * A363050  Lesser of two consecutive integers such that one has more prime
 *          factors (counted with multiplicity), but the other has more
 *          divisors.
 * https://oeis.org/A363050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A363050 – Lesser of two consecutive integers such that one has
   more prime factors (counted with multiplicity) but the other has more
   divisors.  The function returns the *n*-th term (1‑based).  It
   generates the terms lazily and memoises them so that calls in any
   order are efficient.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Primality utilities ----------------------------------------------------- */
#include "../utils/build_primes_upto.h"   /* build_primes_upto() */

/* ------------------------------------------------------------------------- */
/*  Local data: memoised sequence and prime list --------------------------- */
static seq_t *terms        = NULL;   /* stored sequence terms            */
static size_t terms_len    = 0;       /* number of terms currently stored */
static size_t terms_cap    = 0;       /* capacity of the terms array      */
static uint64_t last_seen  = 0;       /* largest integer that has been
                                         inspected so far                */

static uint64_t *primes    = NULL;   /* list of primes for factorisation */
static size_t    prime_cnt = 0;      /* number of primes in the list      */

/* ------------------------------------------------------------------------- */
/*  Integer square‑root (64‑bit, no FP) ------------------------------------ */
static inline uint64_t isqrt64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;   /* sqrt(2^64) = 2^32      */
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t m2 = (__uint128_t)mid * mid;
        if (m2 == x)          return mid;
        if (m2 <  x)  lo = mid + 1;
        else          hi = mid - 1;
    }
    return hi;
}

/* ------------------------------------------------------------------------- */
/*  Ensure the prime list contains all primes ≤ limit --------------------- */
static void ensure_primes_up_to(uint64_t limit)
{
    if (!primes || (prime_cnt == 0) || primes[prime_cnt - 1] < limit) {
        free(primes);
        build_primes_upto(limit, &primes, &prime_cnt);
    }
}

/* ------------------------------------------------------------------------- */
/*  Factorisation helper: gives the number of prime factors (with multiplicity)
    and the number of divisors for a 64‑bit integer. ----------------------- */
static void factor_and_counts(uint64_t n, unsigned *omega, unsigned *tau)
{
    *omega = 0;
    *tau   = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
                (*omega)++;
            }
            *tau *= (exp + 1);
        }
    }
    if (tmp > 1) {            /* remainder is a prime factor         */
        (*omega)++;
        *tau *= 2;
    }
}

/* ------------------------------------------------------------------------- */
/*  Generate terms up to (and including) index wanted (1‑based).             */
static void generate_up_to(idx_t wanted)
{
    /* initialise primes once */
    if (!primes) {
        build_primes_upto(1000000, &primes, &prime_cnt);  /* start 1e6 */
    }

    /* initialise terms array on first call */
    if (!terms) {
        terms_cap = 128;
        terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* scan integers until we have enough terms */
    while ((idx_t)terms_len < wanted) {
        uint64_t cur = last_seen + 1;
        uint64_t next = cur + 1;

        /* need primes up to sqrt(next) */
        uint64_t need = isqrt64(next) + 1;
        ensure_primes_up_to(need);

        unsigned omega_cur, tau_cur, omega_next, tau_next;
        factor_and_counts(cur,  &omega_cur,  &tau_cur);
        factor_and_counts(next, &omega_next, &tau_next);

        bool ok = (omega_cur > omega_next && tau_cur < tau_next) ||
                  (omega_cur < omega_next && tau_cur > tau_next);

        if (ok) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)cur;
        }

        last_seen = cur;
    }
}

/* ------------------------------------------------------------------------- */
/*  Public interface: returns the n‑th term (1‑based).                       */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined for n = 0 */
    generate_up_to(n);
    return terms[n-1];
}
