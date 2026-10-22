/*
 * A339501  a(n) = smallest prime number between prime(n) and 2*prime(n) such
 *          that the arithmetic progression (prime(n), a(n), ...) and which
 *          contains only primes has the maximum length.
 * https://oeis.org/A339501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime q in (prime(n), 2·prime(n)) such that the
 *            arithmetic progression (prime(n), q, ...) contains only primes
 *            and has maximal possible length
 * Range:     u8 n <= 36, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   build_primes_upto, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/get_prime.h"        /* get_prime(n) → nth prime */
#include "../utils/build_primes_upto.h"/* build_primes_upto(limit,&primes,&cnt) */
#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t n) */

/*  A fixed sieve upper bound sufficient for n up to 10000;
    the 10000th prime is 104729, so 2·prime < 210000. */
static const idx_t SIEVE_LIMIT = 10000000ULL;

/* Global data used by the sequence implementation. */
static bool           initialized = false;
static uint64_t      *all_primes   = NULL;   /* array of primes up to SIEVE_LIMIT */
static size_t           all_cnt      = 0;    /* number of primes in all_primes */
static uint8_t        *prime_bits   = NULL;   /* bitset: prime_bits[p] == 1 ⇔ p prime */

/* Initialise the global sieve and prime array once. */
static void init_global(void)
{
    if (initialized)
        return;

    /* Build the list of primes up to SIEVE_LIMIT. */
    build_primes_upto(SIEVE_LIMIT, &all_primes, &all_cnt);

    /* Allocate a simple byte‑array to test primality fast. */
    prime_bits = (uint8_t *)malloc(SIEVE_LIMIT + 1);
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i)
        prime_bits[i] = 0;
    for (size_t i = 0; i < all_cnt; ++i)
        prime_bits[all_primes[i]] = 1;

    initialized = true;
}

/* Fast primality test for numbers up to SIEVE_LIMIT, otherwise delegates
   to a big‑int primality routine. */
static inline bool is_prime_fast(uint64_t x)
{
    if (x <= SIEVE_LIMIT)
        return prime_bits[x] != 0;
    return is_prime_big((seq_t)x);
}

/* Sequence function.  It works for any build that supplies seq_t and idx_t.
   Calls are independent; results are cached via the global static data. */
seq_t a(idx_t n)
{
    init_global();

    /* 1.  The base prime p = prime(n).  */
    uint64_t p = get_prime(n);
    /* 2.  Limit for candidate q:  p < q < 2p  */
    uint64_t limit2p = 2 * p;

    /* Locate the interval of indices in all_primes that fall in (p, 2p). */
    size_t start, end;

    /* Find first prime > p. */
    size_t lo = 0, hi = all_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (all_primes[mid] <= p)
            lo = mid + 1;
        else
            hi = mid;
    }
    start = lo;

    /* Find first prime ≥ 2p. */
    lo = 0; hi = all_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (all_primes[mid] < limit2p)
            lo = mid + 1;
        else
            hi = mid;
    }
    end = lo;             /* end is exclusive */

    /* 3.  Evaluate each candidate q.  */
    int      best_len = 0;
    uint64_t best_q   = 0;

    for (size_t idx = start; idx < end; ++idx) {
        uint64_t q = all_primes[idx];
        uint64_t d = q - p;   /* common difference */

        int len = 2;          /* p and q are already prime */
        uint64_t next = p + 2 * d;

        while (is_prime_fast(next)) {
            ++len;
            next += d;
        }

        if (len > best_len || (len == best_len && q < best_q)) {
            best_len = len;
            best_q   = q;
        }
    }

    return (seq_t)best_q;
}
