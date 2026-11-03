/*
 * A249800  a(n) is the smallest prime q such that n(q+1)+1 is prime, that is,
 *          the smallest prime q such that n = (p-1)/(q+1) with p prime; or
 *          a(n) = -1 if no such q exists.
 * https://oeis.org/A249800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 101 terms through n = 985 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_seq, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249800: a(n) is the smallest prime q such that n*(q+1)+1 is prime,
   or -1 if no such prime exists. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_seq.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static uint64_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static bool primes_initialized = false;
static const idx_t PRIMES_LIMIT = 2000000;

static void
init_primes(void)
{
    if (primes_initialized)
        return;
    build_primes_upto(PRIMES_LIMIT, &prime_cache, &prime_cnt);
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;       /* below OEIS offset – undefined value */

    init_primes();

    /* Check the primes we already have. */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t q = prime_cache[i];
        seq_t p = (seq_t)n * ((seq_t)q + 1) + 1;
        if (is_prime_big(p))
            return (seq_t)q;
    }

    /* If not found, extend the search beyond the initial sieve. */
    uint64_t q_start = prime_cache[prime_cnt - 1] + 1;
    uint64_t q = next_prime(q_start);        /* first prime > q_start */
    const uint64_t MAX_Q = 10000000ULL;     /* practical upper bound */

    while (q <= MAX_Q)
    {
        seq_t p = (seq_t)n * ((seq_t)q + 1) + 1;
        if (is_prime_big(p))
            return (seq_t)q;
        q = next_prime(q + 1);
    }

    /* No suitable q found within the bound. */
    return (seq_t)-1;
}
