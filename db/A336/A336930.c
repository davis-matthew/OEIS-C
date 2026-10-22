/*
 * A336930  Numbers k such that the 2-adic valuation of A003973(k), the sum of
 *          divisors of the prime shifted k is equal to the 2-adic valuation
 *          of the number of divisors of k.
 * https://oeis.org/A336930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest number k such that * v₂(σ( prime‑shift(k) )) =
 *            v₂( τ(k) ) * where prime‑shift(k) is obtained by replacing each
 *            prime * factor p of k by the next prime after p
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Parameters for the sieve */
#define SIEVE_LIMIT 500000

/* Global tables */
static idx_t        *spf          = NULL;          /* smallest prime factor */
static idx_t        *nextPrime    = NULL;          /* next prime after p for prime p */
static idx_t        *prime_list   = NULL;          /* list of primes up to SIEVE_LIMIT */
static size_t        primes_cnt   = 0;
static bool          init_done    = false;

/* Sequence storage */
static seq_t        *seq_cache    = NULL;
static size_t        seq_len      = 0;
static size_t        seq_cap      = 0;
static idx_t        curr_k        = 1;  /* candidate k to test */

/* ------------------------------------------------------------------ */
/* Utility: compute v₂ of a positive 64‑bit integer (ctz)             */
static inline unsigned v2_u64(uint64_t x) { return __builtin_ctzll(x); }

/* ------------------------------------------------------------------ */
/* Sieve of Eratosthenes with linear time SPF and next‑prime table   */
static void build_sieve(void)
{
    uint64_t lim = SIEVE_LIMIT;
    spf       = (idx_t*)calloc(lim + 1, sizeof(idx_t));
    prime_list= (idx_t*)malloc(lim * sizeof(idx_t));   /* oversize, enough for all primes */
    nextPrime = (idx_t*)calloc(lim + 1, sizeof(idx_t));

    size_t pc = 0;
    for (idx_t i = 2; i <= lim; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            prime_list[pc++] = i;
        }
        for (size_t j = 0; j < pc &&
                          prime_list[j] <= spf[i] &&
                          (uint64_t)i * prime_list[j] <= lim; ++j) {
            spf[i * prime_list[j]] = prime_list[j];
        }
    }
    primes_cnt = pc;

    /* Build nextPrime table: for each prime p, nextPrime[p] = next prime after p */
    for (size_t i = 0; i + 1 < primes_cnt; ++i) {
        idx_t p  = prime_list[i];
        idx_t np = prime_list[i + 1];
        nextPrime[p] = np;
    }
    /* The last prime has no following prime inside the limit; entry stays zero */
}

/* ------------------------------------------------------------------ */
/* Check whether a candidate k satisfies the defining property        */
static bool qualifies(idx_t k)
{
    idx_t n = k;
    int sigma_v2 = 0;
    int tau_v2   = 0;

    while (n > 1) {
        idx_t p = spf[n];
        idx_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        idx_t exp = e + 1;                   /* exponent in divisor count and sigma */
        unsigned v2_exp = v2_u64(exp);
        tau_v2 += v2_exp;

        idx_t q = nextPrime[p];
        if (q == 0)        /* next prime beyond sieve limit; should not happen */
            return false;

        if ((exp & 1) == 0) {                /* exp is even */
            unsigned v2_q1 = v2_u64(q + 1);
            sigma_v2 += (int)(v2_q1 + v2_exp - 1);
        }
        /* if exp is odd, contribution to sigma_v2 is 0 */
    }
    return sigma_v2 == tau_v2;
}

/* ------------------------------------------------------------------ */
/* Ensure sequence cache has space for needed terms                    */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* ------------------------------------------------------------------ */
/* Public function required by the build                              */
seq_t a(idx_t n)
{
    if (!init_done) {
        build_sieve();
        init_done = true;
    }

    while (seq_len < n) {
        while (!qualifies(curr_k)) ++curr_k;   /* skip non‑qualifiers */

        /* add curr_k to cache */
        ensure_seq_cap(seq_len + 1);
        seq_cache[seq_len++] = (seq_t)curr_k;
        ++curr_k;
    }

    return seq_cache[n - 1];
}
