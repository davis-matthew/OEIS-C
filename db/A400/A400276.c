/*
 * A400276  Numbers k such that A046660(k) >= A056169(k).
 * https://oeis.org/A400276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all positive integers k for which
   Sum_{prime p | k} max(exp_p(k)-1,0)  >=  number of distinct prime divisors
   of k.  Equivalently, if k = ∏ p_i^{e_i} then
   Σ_{e_i≥2} e_i  >=  r, where r=#distinct primes.  This function generates
   the sequence by testing successive integers until the desired index is
   reached.  The implementation uses a simple trial‑division prime sieve
   and memoises found terms for repeated calls.  The algorithm is
   O(n⋅sqrt(a(n))) which is well below the 2‑second limit for n≈10000.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------- Prime generation --------------------------- */

static uint32_t *primes      = NULL;   /* list of primes for trial division  */
static size_t   primes_cnt  = 0;
static size_t   primes_cap  = 0;

/* Ensure space for at least new_len primes in primes[]. */
static void grow_primes(size_t new_len)
{
    if (new_len <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 32;
    while (new_cap < new_len) new_cap *= 2;
    primes = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    primes_cap = new_cap;
}

/* Return integer square root (floor) of a 64‑bit unsigned integer. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = 0, bit = 1ULL << 62;
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else
            x >>= 1;
        bit >>= 2;
    }
    return x;
}

/* Populate the primes[] list up to at least 'limit'. */
static void ensure_primes_up_to(uint64_t limit)
{
    if (primes_cnt && (uint64_t)primes[primes_cnt-1] >= limit)
        return;
    uint64_t candidate = primes_cnt ? (uint64_t)primes[primes_cnt-1] + 1 : 2;
    while (candidate <= limit) {
        bool is_prime = true;
        uint64_t sq = isqrt_u64(candidate);
        for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] <= sq; ++i) {
            if (candidate % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            grow_primes(primes_cnt + 1);
            primes[primes_cnt++] = (uint32_t)candidate;
        }
        ++candidate;
    }
}

/* ------------------------- Sequence generation --------------------- */

static seq_t *terms      = NULL;   /* stored sequence terms */
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static uint64_t next_candidate = 1;   /* next integer to test for membership */

/* Ensure space for at least new_len terms. */
static void grow_terms(size_t new_len)
{
    if (new_len <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 32;
    while (new_cap < new_len) new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Test whether integer k satisfies the membership condition. */
static bool is_sequence_term(uint64_t k)
{
    if (k == 1) return true;            /* 1 is always a term */

    uint64_t n = k;
    uint64_t sum_ge2 = 0;   /* Σ e_i for primes with exponent ≥ 2 */
    uint64_t distinct = 0;   /* r = number of distinct primes */
    uint64_t sqrt_n = isqrt_u64(n);

    ensure_primes_up_to(sqrt_n);

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] <= sqrt_n; ++i) {
        uint64_t p = primes[i];
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            ++distinct;                   /* one new distinct prime */
            if (e >= 2) sum_ge2 += e;     /* contributions from exponent ≥ 2 */
            sqrt_n = isqrt_u64(n);
        }
    }
    if (n > 1) {   /* remaining prime factor has exponent 1 */
        ++distinct;
    }
    return sum_ge2 >= distinct;
}

/* -------------------------- Public API -------------------------------- */

seq_t a(idx_t n)
{
    /* Compute enough terms to reach index n. */
    while (terms_len < n) {
        if (is_sequence_term(next_candidate)) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return terms[n-1];
}
