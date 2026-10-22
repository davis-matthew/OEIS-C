/*
 * A349897  Start with b(0)=4. a(n) is the smallest integer such that
 *          b(n)=a(n)*b(n-1) has twice as many divisors as b(n-1) has.
 * https://oeis.org/A349897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A349897:  a(1) = 3, a(2) = 5, …
   This implementation keeps track of the factorisation of the current
   product b(n)=4*∏_{k=1}^{n} a(k).  For each new term it searches
   for the smallest integer a for which the divisor count of b
   doubles.  The search is carried out by examining all integers
   ≥2, factorising them and checking whether the multiplicative
   increase of divisor count equals 2.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/seq_gcd.h"

/* Prime factorisation helper structures */
typedef struct {
    seq_t prime;
    seq_t exp;
} prime_exp_t;

/* Dynamic arrays for primes and exponents */
static uint64_t *primes      = NULL;
static size_t    primes_cap  = 0;
static size_t    primes_cnt  = 0;
static bool      primes_gen  = false;

static prime_exp_t *primeExp = NULL;
static size_t      primeExp_cap = 0;
static size_t      primeExp_cnt = 0;

static seq_t *a_cache   = NULL;
static size_t   a_cache_cap   = 0;
static idx_t    a_cache_len   = 0;

/* ------------------------------------------------------------------ */
/* Prime sieve – generates primes up to 2,000,000 (more than enough for
   the expected search range). */
static void generate_primes(void)
{
    if (primes_gen) return;

    const size_t LIMIT = 2000000;
    bool *is_comp = (bool*)calloc(LIMIT + 1, 1);
    primes_cap = 100000;
    primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    primes_cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = i;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    primes_gen = true;
}

/* ------------------------------------------------------------------ */
/* Helper to factorise a 64‑bit integer using the precomputed primes. */
#define FACTOR_BUF_SZ 64
static void factor_uint64(uint64_t n,
                          uint64_t *pbuf, uint64_t *ebuf, size_t *cnt)
{
    *cnt = 0;
    uint64_t temp = n;
    size_t i = 0;
    while (i < primes_cnt && primes[i] * primes[i] <= temp) {
        if (temp % primes[i] == 0) {
            uint64_t f = 0;
            while (temp % primes[i] == 0) {
                temp /= primes[i];
                ++f;
            }
            pbuf[*cnt] = primes[i];
            ebuf[*cnt] = f;
            ++(*cnt);
        }
        ++i;
    }
    if (temp > 1) {           /* remaining prime factor */
        if (*cnt == FACTOR_BUF_SZ) {
            /* Should never happen for expected values */
            pbuf[*cnt] = temp;
            ebuf[*cnt] = 1;
        } else {
            pbuf[*cnt] = temp;
            ebuf[*cnt] = 1;
        }
        ++(*cnt);
    }
}

static seq_t get_exp(uint64_t p)
{
    for (size_t i = 0; i < primeExp_cnt; ++i) {
        if (primeExp[i].prime == (seq_t)p)
            return primeExp[i].exp;
    }
    return (seq_t)0;
}

static void add_factor(uint64_t p, uint64_t f)
{
    size_t idx = primeExp_cnt;
    for (size_t i = 0; i < primeExp_cnt; ++i) {
        if (primeExp[i].prime == (seq_t)p) {
            idx = i;
            break;
        }
    }
    seq_t ff = (seq_t)f;
    if (idx == primeExp_cnt) {           /* new prime */
        if (primeExp_cnt == primeExp_cap) {
            primeExp_cap = (primeExp_cap == 0) ? 4 : primeExp_cap * 2;
            primeExp = (prime_exp_t*)realloc(primeExp,
                                             primeExp_cap * sizeof(prime_exp_t));
        }
        primeExp[primeExp_cnt].prime = (seq_t)p;
        primeExp[primeExp_cnt].exp   = ff;
        ++primeExp_cnt;
    } else {                             /* existing prime */
        primeExp[idx].exp += ff;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialise data structures on first call */
    if (!primes_gen) generate_primes();

    if (primeExp_cnt == 0) {             /* initial factorisation of b(0)=4=2^2 */
        primeExp_cap = 4;
        primeExp = (prime_exp_t*)malloc(primeExp_cap * sizeof(prime_exp_t));
        primeExp[0].prime = (seq_t)2;
        primeExp[0].exp   = (seq_t)2;
        primeExp_cnt = 1;
    }

    /* extend cache if necessary */
    if (n > a_cache_len) {
        size_t new_cap = a_cache_cap;
        if (new_cap == 0) new_cap = 64;
        while (n >= new_cap) new_cap <<= 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        memset(a_cache + a_cache_cap, 0, (new_cap - a_cache_cap) * sizeof(seq_t));
        a_cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = a_cache_len + 1; i <= n; ++i) {
        uint64_t cand = 2;
        while (1) {
            uint64_t cand_primes[FACTOR_BUF_SZ];
            uint64_t cand_exps  [FACTOR_BUF_SZ];
            size_t   cand_cnt   = 0;
            factor_uint64(cand, cand_primes, cand_exps, &cand_cnt);

            seq_t numer  = (seq_t)1;
            seq_t denom  = (seq_t)1;
            for (size_t j = 0; j < cand_cnt; ++j) {
                uint64_t   p = cand_primes[j];
                uint64_t   f = cand_exps[j];
                seq_t e = get_exp(p);

                seq_t num_factor = e + (seq_t)f + (seq_t)1;
                seq_t den_factor = e + (seq_t)1;

                numer *= num_factor;
                denom *= den_factor;

                seq_t g = seq_gcd(numer, denom);
                numer /= g;
                denom /= g;
            }
            /* final reduction */
            seq_t g2 = seq_gcd(numer, denom);
            numer /= g2;
            denom /= g2;

            if (denom == (seq_t)1 && numer == (seq_t)2) {
                /* cand is the desired a(i) */
                a_cache[i] = (seq_t)cand;
                /* update exponents of current product b(i) */
                for (size_t j = 0; j < cand_cnt; ++j)
                    add_factor(cand_primes[j], cand_exps[j]);
                break;
            }
            ++cand;
        }
    }
    a_cache_len = n;
    return a_cache[n];
}
