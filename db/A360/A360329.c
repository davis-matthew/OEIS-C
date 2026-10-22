/*
 * A360329  a(n) is the largest divisor of n that has only prime factors that
 *          are not prime-indexed primes.
 * https://oeis.org/A360329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n divided by the product of its prime‑indexed prime *
 *            factors. A prime is “prime‑indexed” if its index in the sequence
 *            * of primes is also prime. The sequence is multiplicative with *
 *            a(p^e) = 1 if p is prime‑indexed, and p^e otherwise
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   small_is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/small_is_prime.h"

/* --------- Local utilities --------------------------------------- */

/* --------- Prime generation and caching -------------------------- */

#define MAX_PRIMES 20000                /* enough for n up to ~ 1e6 */

static bool primes_ready          = false;
static uint64_t primes[MAX_PRIMES];
static size_t   primes_cnt         = 0;
static bool     prime_indexed[MAX_PRIMES];   /* true if index is prime */

/* Generate primes up to 'limit' (inclusive).  Sieve of Eratosthenes. */
static void ensure_primes(uint64_t limit)
{
    if (primes_ready && primes[primes_cnt - 1] >= limit)
        return;                /* already covered */

    /* Allocate workspace */
    size_t size = (size_t)(limit + 1);
    bool *mark = (bool*)calloc(size, sizeof(bool));
    if (!mark) return;            /* fallback: leave empty */

    primes_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!mark[i]) {
            if (primes_cnt >= MAX_PRIMES) break;
            primes[primes_cnt++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    mark[j] = true;
            }
        }
    }
    free(mark);

    /* Mark prime-indexed primes */
    for (size_t i = 0; i < primes_cnt; ++i)
        prime_indexed[i] = small_is_prime((int)(i + 1));

    primes_ready = true;
}

/* Check whether a given prime 'p' is prime‑indexed.
 * Assumes 'p' is prime. */
static bool is_prime_indexed(uint64_t p)
{
    /* Ensure we have primes at least up to p */
    ensure_primes(p);
    /* Binary search for index of p */
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint64_t q = primes[mid];
        if (q == p)
            return prime_indexed[mid];
        if (q < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;   /* should not happen for prime inputs */
}

/* --------- Main sequence function -------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* outside offset, safety */

    uint64_t nn = (uint64_t)n;
    uint64_t limit = nn ? nn : 2;
    ensure_primes(limit);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= nn; ++i) {
        uint64_t p = primes[i];
        if (nn % p != 0) continue;
        uint64_t exp = 0;
        while (nn % p == 0) {
            nn /= p;
            ++exp;
        }
        if (!is_prime_indexed(p)) {
            seq_t p_pow = (seq_t)1;
            for (uint64_t j = 0; j < exp; ++j)
                p_pow *= (seq_t)p;
            result *= p_pow;
        }
    }

    if (nn > 1) {   /* remaining prime factor */
        if (!is_prime_indexed(nn)) {
            result *= (seq_t)nn;
        }
    }

    return result;
}
