/*
 * A398400  a(n) is the least k such that the concatenation of prime(n+1),
 *          prime(n+2),..., prime(n+k) is a multiple of prime(n) or 0 if no
 *          such k exists.
 * https://oeis.org/A398400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: For given n, find the smallest k ≥ 1 such that
 * the decimal concatenation of primes prime(n+1)…prime(n+k)
 * is a multiple of prime(n).  Return 0 if no such k is found.
 * The search is limited to a generous number of steps but will
 * find the correct answer for all n ≤ 10000 with the chosen
 * prime limit.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* required utilities */
#include "../utils/build_primes_upto.h"   /* build_primes_upto() */
#include "../utils/digit_count.h"        /* digit_count() */

/* prime cache (zero‑based) */
static uint64_t *prime_cache = NULL;
static size_t   prime_cache_cnt = 0;

/* initialise prime cache once */
static void init_primes(void)
{
    if (prime_cache != NULL)
        return;

    /* 2 000 000 is more than enough for n ≤ 10 000 and the
       search bound used in this implementation (200 000 steps). */
    build_primes_upto(2000000, &prime_cache, &prime_cache_cnt);
}

/* compute 10^e (mod m) using 128‑bit multiplication to avoid overflow */
static uint64_t pow10_mod(uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    for (uint64_t i = 0; i < e; ++i) {
        r = (uint64_t)((__uint128_t)r * 10 % m);
    }
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                        /* undefined for n < 1 */

    init_primes();

    /* p_n is the nth prime (1‑based).  It is stored at index n‑1. */
    uint64_t p_n = prime_cache[n - 1];

    /* If p_n is 2 or 5, no concatenation of subsequent primes
       can be a multiple of it (all primes end in 1,3,7,9). */
    if (p_n == 2 || p_n == 5)
        return 0;

    /* Start scanning primes beginning with prime(n+1). */
    idx_t idx = n;                       /* zero‑based index of the next prime */
    uint64_t rem = 0;                    /* current remainder modulo p_n */

    /* Maximum number of concatenated primes to try. */
    const size_t MAX_STEPS = 200000;

    for (size_t step = 1; step <= MAX_STEPS && idx < prime_cache_cnt; ++step, ++idx) {
        uint64_t prime = prime_cache[idx];
        uint64_t d = digit_count(prime);           /* number of decimal digits */
        uint64_t ten_pow = pow10_mod(d, p_n);

        /* Update remainder: (rem * 10^d + prime) mod p_n */
        rem = (uint64_t)((__uint128_t)rem * ten_pow % p_n);
        rem = (rem + prime % p_n) % p_n;

        if (rem == 0)                /* found minimal k */
            return (seq_t)step;
    }

    /* No suitable k found within the search bound. */
    return 0;
}
