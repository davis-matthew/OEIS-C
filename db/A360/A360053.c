/*
 * A360053  Primes p such that each prime < p in the prime factorization of
 *          2^(p-1) - 1 has exponent 1.
 * https://oeis.org/A360053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A360053: a(n) = the n-th prime p such that every prime
 * q < p appearing in 2^(p-1)-1 has exponent 1.
 * The function below computes this value using a sieve of primes
 * and an efficient test based on the Lifting‑Exponent Lemma.
 *
 * It precomputes all primes up to a generous limit, then
 * for each candidate prime p it checks that no prime q < p
 * satisfies q^2 | 2^(p-1)-1.  The test uses the fact that
 * for an odd prime q,
 *   q^2 | 2^(p-1)-1   ⇔   ord_q(2) | (p-1)  and
 *                           (p-1)/ord_q(2) is divisible by q,
 *   or the stronger condition that 2^ord_q(2) ≡ 1 (mod q^2)
 *   (a Wieferich prime case).  The order ord_q(2) is pre‑computed
 *   for all primes up to the sieve limit.
 *
 * The implementation is constant‑state, handles calls in arbitrary
 * order, and is fast enough for n up to about 10000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/factor_u64.h"

/* ---------- helper: modular exponentiation (uint64_t) ---------- */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* ---------- sieve of primes up to LIMIT ---------- */
#define SIEVE_LIMIT 2000000  /* sufficient for n up to ~10k */

static uint32_t *primes = NULL;        /* array of primes up to LIMIT */
static size_t   primes_count = 0;
static size_t   prime_idx    = 0;       /* next prime to test */
static uint64_t *ord_cache   = NULL;    /* order of 2 modulo each prime */

/* binary search: index of prime (must exist) */
static size_t prime_index_of(uint64_t p)
{
    size_t lo = 0, hi = primes_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p)
            return mid;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (size_t)-1;   /* not found – should never happen */
}

/* compute ord_q(2) for an odd prime q */
static uint64_t compute_ord(uint32_t q)
{
    if (q == 2) return 1;          /* not used, but harmless */

    uint64_t n = (uint64_t)q - 1;
    uint64_t fac[32];
    uint64_t exp[32];
    size_t cnt = 0;
    factor_u64(n, fac, exp, &cnt);

    uint64_t ord = n;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t r = fac[i];
        while ((ord % r) == 0) {
            uint64_t cand = ord / r;
            if (mod_pow_uint64(2, cand, q) == 1)
                ord = cand;
            else
                break;
        }
    }
    return ord;
}

/* initialisation: sieve and pre‑compute orders */
static void init_sequence(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* sieve */
    bool *sieve = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    memset(sieve, 1, (SIEVE_LIMIT + 1));
    sieve[0] = sieve[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (sieve[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = 0;
    /* count primes */
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve[i]) primes_count++;
    /* store primes */
    primes = (uint32_t *)malloc(primes_count * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (sieve[i])
            primes[idx++] = i;
    free(sieve);

    /* pre‑compute orders */
    ord_cache = (uint64_t *)malloc(primes_count * sizeof(uint64_t));
    for (size_t i = 0; i < primes_count; ++i)
        ord_cache[i] = compute_ord(primes[i]);

    /* allocate cache for good primes (will grow on demand) */
    /* will be initialised in first call to a() */
}

/* dynamic array of good primes */
static seq_t *good_primes = NULL;
static size_t  good_count = 0;
static size_t  good_cap   = 0;

static void append_good(seq_t val)
{
    if (good_count == good_cap) {
        size_t new_cap = good_cap ? good_cap << 1 : 4;
        seq_t *tmp = (seq_t *)realloc(good_primes, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* out of memory */
        good_primes = tmp;
        good_cap    = new_cap;
    }
    good_primes[good_count++] = val;
}

/* test if prime p satisfies the condition */
static bool is_good_prime(uint32_t p)
{
    if (p == 2) return true;          /* 2 always good */

    uint64_t n = (uint64_t)p - 1;
    uint64_t fac[32];
    uint64_t exp[32];
    size_t cnt = 0;
    factor_u64(n, fac, exp, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = fac[i];
        if (q == 2) continue;          /* skip factor 2 */

        size_t qi = prime_index_of(q);
        uint64_t ord = ord_cache[qi];

        /* if 2^ord ≡ 1 (mod q^2) then q^2 already divides 2^ord-1,
         * hence divides 2^(p-1)-1. */
        if (mod_pow_uint64(2, ord, q * q) == 1)
            return false;

        if ((n % ord) != 0) continue;   /* ord does not divide p-1 */

        uint64_t m = n / ord;
        if (m % q == 0)
            return false;                /* exponent of q is at least 2 */
    }
    return true;
}

/* fill good_primes until it contains at least n entries */
static void ensure_good(idx_t n)
{
    init_sequence();
    while ((idx_t)good_count < n) {
        if (prime_idx >= primes_count) {
            /* should not happen with SIEVE_LIMIT set large enough */
            exit(1);
        }
        uint32_t p = primes[prime_idx++];
        if (is_good_prime(p))
            append_good((seq_t)p);
    }
}

/* -----------------------------------------------------------------
 *  Interface function as required by the sequence generator.
 * ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_good(n);
    return good_primes[n - 1];
}
