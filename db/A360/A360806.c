/*
 * A360806  a(0) = 1; for n >= 1, a(n) is the least integer k > a(n-1) such
 *          that k / A000005(k) = a(n-1).
 * https://oeis.org/A360806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A360806
 * a(0) = 1;
 * a(n) = least integer k > a(n-1) such that k / A000005(k) = a(n-1)
 * (A000005 is the divisor-count function τ(k)).
 * The implementation builds the sequence iteratively, keeping the prime
 * factorisation of the latest term to enable fast divisor‑count checks
 * for candidate k.  It works for any seq_t / idx_t supplied by the
 * build environment, including arbitrary‑precision seq_t.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*   Prime sieve (used for trial division in factorisation)          */
/* ------------------------------------------------------------------ */

#define MAX_PRIME 2000000u         /* sieve limit for small primes */

static uint32_t *primes = NULL;    /* array of primes up to MAX_PRIME      */
static size_t   prime_count = 0;   /* number of primes found               */

static void init_primes(void)
{
    if (primes != NULL)
        return;                    /* already initialised                 */

    /* simple sieve of Eratosthenes */
    size_t limit = MAX_PRIME;
    uint8_t *is_prime = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    for (size_t i = 2; i <= limit; ++i)
        is_prime[i] = 1;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++prime_count;

    primes = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    size_t pidx = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i])
            primes[pidx++] = (uint32_t)i;
    }

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/*   Factorisation of an uint64_t using the precomputation primes     */
/* ------------------------------------------------------------------ */

static void factor_uint64(uint64_t n, uint64_t *fpr,
                          uint32_t *fct, size_t *flen)
{
    *flen = 0;
    if (n < 2)
        return;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n)
            break;
        if (n % p == 0) {
            uint32_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            fpr[*flen] = p;
            fct[*flen] = e;
            ++(*flen);
        }
    }
    if (n > 1) {          /* remaining prime factor */
        fpr[*flen] = n;
        fct[*flen] = 1;
        ++(*flen);
    }
}

/* ------------------------------------------------------------------ */
/*   Storage for current term’s prime factorisation                     */
/* ------------------------------------------------------------------ */

static uint64_t *cur_primes = NULL;
static uint32_t *cur_exps   = NULL;
static size_t   cur_len     = 0;
static size_t   cur_cap     = 0;

/* ------------------------------------------------------------------ */
/*   Update divisor count for candidate t = candidate prime factors   */
/* ------------------------------------------------------------------ */

static seq_t compute_tau(const uint64_t *cp, const uint32_t *ce,
                         size_t clen,
                         const uint64_t *tp, const uint32_t *te,
                         size_t tlen)
{
    seq_t tau = (seq_t)1;

    /* primes present in current term */
    for (size_t i = 0; i < clen; ++i) {
        uint64_t p = cp[i];
        uint32_t e_cur = ce[i];
        uint32_t e_t   = 0;
        for (size_t j = 0; j < tlen; ++j)
            if (tp[j] == p) {
                e_t = te[j];
                break;
            }
        seq_t exp_total = (seq_t)(e_cur + e_t + 1);
        tau = tau * exp_total;
    }

    /* primes only in candidate t */
    for (size_t i = 0; i < tlen; ++i) {
        uint64_t p = tp[i];
        bool in_cur = false;
        for (size_t j = 0; j < clen; ++j) {
            if (cp[j] == p) {
                in_cur = true;
                break;
            }
        }
        if (!in_cur) {
            seq_t exp_total = (seq_t)(te[i] + 1);
            tau = tau * exp_total;
        }
    }

    return tau;
}

/* ------------------------------------------------------------------ */
/*   Merge candidate factorisation into current term                  */
/* ------------------------------------------------------------------ */

static void merge_factors(const uint64_t *tp, const uint32_t *te,
                          size_t tlen)
{
    for (size_t i = 0; i < tlen; ++i) {
        uint64_t p = tp[i];
        uint32_t e = te[i];
        /* search in current */
        size_t pos = 0;
        while (pos < cur_len && cur_primes[pos] != p)
            ++pos;
        if (pos < cur_len) {
            cur_exps[pos] += e;
        } else {
            /* grow arrays if needed */
            if (cur_len == cur_cap) {
                cur_cap = cur_cap ? cur_cap * 2 : 4;
                cur_primes = (uint64_t *)realloc(cur_primes,
                                                cur_cap * sizeof(uint64_t));
                cur_exps   = (uint32_t *)realloc(cur_exps,
                                                cur_cap * sizeof(uint32_t));
            }
            cur_primes[cur_len] = p;
            cur_exps[cur_len]   = e;
            ++cur_len;
        }
    }
}

/* ------------------------------------------------------------------ */
/*   Sequence cache                                                 */
/* ------------------------------------------------------------------ */

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

/* ------------------------------------------------------------------ */
/*   Main sequence function                                         */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init_primes();

    /* initialise on first call */
    if (cache_size == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;            /* a(0) */
        cache_size = 1;

        cur_len = 0;
        cur_cap = 0;
        cur_primes = NULL;
        cur_exps   = NULL;
    }

    /* extend the cache up to the required index */
    while (cache_size <= (size_t)n) {
        /* search for the smallest t > 1 such that tau(prev * t) == t */
        uint64_t t = 2;
        uint64_t cand_pr[64];
        uint32_t cand_exp[64];
        size_t cand_len;
        for (;;) {
            factor_uint64(t, cand_pr, cand_exp, &cand_len);
            seq_t tau = compute_tau(cur_primes, cur_exps, cur_len,
                                    cand_pr, cand_exp, cand_len);
            if (tau == (seq_t)t)
                break;
            ++t;
        }

        /* compute new term: val = prev * t */
        seq_t prev_val = cache[cache_size - 1];
        seq_t new_val  = prev_val * (seq_t)t;

        /* store in cache */
        if (cache_size == cache_cap) {
            cache_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size] = new_val;
        ++cache_size;

        /* update the factorisation of the current term */
        merge_factors(cand_pr, cand_exp, cand_len);
    }

    return cache[(size_t)n];
}
