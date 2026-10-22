/*
 * A328266  a(n) is the least k > 0 such that prime(n) AND prime(n+k) <= 1
 *          (where prime(n) denotes the n-th prime number and AND denotes the
 *          bitwise AND operator).
 * https://oeis.org/A328266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A328266:
   a(n) = smallest k > 0 such that prime(n) & prime(n+k) <= 1
   (prime(n) is the n‑th prime, bitwise AND).  The function is
   computed by generating primes on demand and searching for the
   smallest admissible k. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/* Prime cache (uint64_t)                                         */
/* ------------------------------------------------------------ */
static uint64_t      *prime_cache = NULL;  /* primes[0] = 2               */
static size_t        prime_cnt   = 0;      /* number of primes stored     */
static size_t        prime_cap   = 0;      /* capacity of prime_cache     */
static uint64_t      last_cand   = 1;      /* last tested candidate       */

/* Grow prime_cache to hold at least new_cap elements */
static void grow_prime_cache(size_t new_cap)
{
    if (prime_cap >= new_cap)
        return;
    size_t cap = prime_cap ? prime_cap * 2 : 128;
    while (cap < new_cap)
        cap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, cap * sizeof(uint64_t));
    /* In case realloc fails, the old memory remains unchanged; this
       situation is extremely unlikely for the data sizes used.          */
    if (tmp)
        prime_cache = tmp;
    prime_cap = cap;
}

/* Test whether candidate is prime using the primes already cached. */
static bool candidate_is_prime(uint64_t cand)
{
    if (cand < 2)
        return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > cand)
            break;
        if (cand % p == 0)
            return false;
    }
    return true;
}

/* Ensure that prime_cache holds primes up to index target (1‑based). */
static void ensure_primes_upto(idx_t target)
{
    /* target is at least 1 (called with n >= 1). */
    while (prime_cnt < (size_t)target) {
        uint64_t cand = ++last_cand;
        while (!candidate_is_prime(cand))
            ++cand;
        grow_prime_cache(prime_cnt + 1);
        prime_cache[prime_cnt++] = cand;
        last_cand = cand;
    }
}

/* ------------------------------------------------------------ */
/* Main sequence function                                         */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                 /* sequence is 1‑based; undefined for 0 */
        return (seq_t)0;

    /* Get prime(n) */
    ensure_primes_upto(n);
    uint64_t p_n = prime_cache[(size_t)(n - 1)];

    /* Find the smallest k > 0 with (p_n & prime(n+k)) <= 1 */
    idx_t k = 1;
    for (;;) {
        idx_t idx_next = n + k;
        ensure_primes_upto(idx_next);
        uint64_t p_next = prime_cache[(size_t)(idx_next - 1)];
        if ((p_n & p_next) <= 1)
            return (seq_t)k;
        ++k;
    }
}
