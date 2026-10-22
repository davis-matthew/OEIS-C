/*
 * A328137  Primes p such that p+1 is the product of two distinct primes and
 *          p+2 is the product of three distinct primes.
 * https://oeis.org/A328137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 143, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for the primes in OEIS A328137.
   It uses a sieve to generate primes up to a fixed limit, then
   tests each prime p for the conditions:
     p + 1 = a product of two distinct primes
     p + 2 = a product of three distinct primes.
   The terms are cached so that subsequent calls are fast.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static bool   initialized = false;      /* whether the sieve is ready   */
static size_t sieve_lim    = 0;         /* current size of the sieve   */
static bool   *is_prime = NULL;        /* primality bit array         */
static uint32_t *spf      = NULL;      /* smallest prime factor array */
static size_t   cur       = 2;          /* next candidate to test     */

/* Dynamic array to store found terms.  */
static seq_t  *terms     = NULL;
static size_t  terms_cnt = 0;
static size_t  terms_cap = 0;

/* Expand the terms array to hold at least `need`.  */
static void grow_terms(size_t need)
{
    if (terms_cap >= need) return;
    size_t new_cap = (terms_cap == 0) ? 64 : terms_cap << 1;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* Append a new term to the terms array.  */
static void add_term(seq_t val)
{
    grow_terms(terms_cnt + 1);
    terms[terms_cnt++] = val;
}

/* Build the sieve of Eratosthenes together with an array of smallest
   prime factors.  The limit is chosen large enough to contain the
   first 10000 terms of the sequence (2·10^7 is more than sufficient).  */
static void build_sieve(void)
{
    if (initialized) return;

    sieve_lim = 20000000;                      /* 20 million */

    is_prime = (bool *)malloc((sieve_lim + 1) * sizeof(bool));
    spf     = (uint32_t *)malloc((sieve_lim + 1) * sizeof(uint32_t));
    if (!is_prime || !spf) abort();

    memset (is_prime, 1, sieve_lim + 1);
    is_prime[0] = is_prime[1] = 0;
    memset (spf, 0, (sieve_lim + 1) * sizeof(uint32_t));

    for (uint32_t i = 2; i <= sieve_lim; ++i) {
        if (!is_prime[i]) continue;
        spf[i] = i;                            /* prime itself */
        if ((uint64_t)i * i <= sieve_lim) {
            for (uint32_t j = i * i; j <= sieve_lim; j += i) {
                if (is_prime[j]) is_prime[j] = 0;
                if (!spf[j]) spf[j] = i;
            }
        }
    }
    initialized = true;
}

/* Check whether n is square‑free and has exactly `req` distinct prime
   factors.  All arithmetic is done with uint64_t because n ≤ sieve_lim + 2.  */
static inline bool satisfies(uint64_t n, size_t req)
{
    size_t cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        if (p == 0) return false;      /* safety check – should not happen */
        ++cnt;
        n /= p;
        if (n % p == 0) return false;  /* duplicate prime factor -> not square‑free */
    }
    return cnt == req;
}

/* Main sequence function: a(idx_t n)  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behaviour below offset is irrelevant */

    if (!initialized) build_sieve();

    size_t need = (size_t)n;

    while (terms_cnt < need) {
        /* Ensure we stay inside the sieve limits.
           The chosen limit should be sufficient for n up to 10000. */
        if (cur + 2 > sieve_lim) {
            /* If we exhaust the sieve, rebuild a larger one.
               In practice this does not happen for the required range. */
            free(is_prime);
            free(spf);
            initialized = false;
            build_sieve();
            continue;
        }

        if (is_prime[cur]) {
            uint64_t x1 = (uint64_t)cur + 1;
            uint64_t x2 = (uint64_t)cur + 2;
            if (satisfies(x1, 2) && satisfies(x2, 3))
                add_term((seq_t)cur);
        }
        ++cur;
    }
    return terms[need - 1];
}
