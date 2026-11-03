/*
 * A400325  a(n) is the number of ordered pairs (x,y) modulo prime(n)
 *          satisfying the equation of the Lemniscate of Bernoulli (x^2+y^2)^2
 *          = x^2-y^2.
 * https://oeis.org/A400325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = 4 for n=1; for n>1 let p = prime(n); return (p % 8 == 1)
 *            ? (p - 4) : p
 * Range:     u8 n <= 55, u16 n <= 6543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9677 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Test if a uint32_t value is prime using the list of known primes. */
static int _is_prime_uint32(uint32_t n, const uint32_t *prs, size_t cnt)
{
    if (n < 2) return 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p = prs[i];
        uint64_t prod = (uint64_t)p * p;
        if (prod > n) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    /* Static cache of primes generated so far. */
    static uint32_t *primes = NULL;
    static size_t    primes_cnt = 0;   /* number of primes stored   */
    static size_t    primes_cap = 0;   /* allocated array capacity  */

    size_t target = (size_t)n;
    while (primes_cnt < target) {
        /* Grow the storage if needed. */
        if (primes_cnt == primes_cap) {
            size_t new_cap = primes_cap ? primes_cap * 2 : 16;
            uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
            if (!tmp) return (seq_t)0;            /* allocation failed */
            primes = tmp;
            primes_cap = new_cap;
        }

        /* Find the next prime after the last one in the cache. */
        uint32_t start = (primes_cnt == 0) ? 2 : primes[primes_cnt - 1] + 1;
        for (uint32_t cand = start;; ++cand) {
            if (_is_prime_uint32(cand, primes, primes_cnt)) {
                primes[primes_cnt++] = cand;
                break;
            }
        }
    }

    uint32_t p = primes[n - 1];  /* the nth prime (prime(1) = 2) */

    if (n == 1) {
        return (seq_t)4;                          /* a(1) = 4 */
    }
    if ((p & 7U) == 1U) {                        /* p % 8 == 1 */
        return (seq_t)(p - 4);
    }
    return (seq_t)p;
}
