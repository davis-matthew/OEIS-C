/*
 * A365173  The number of divisors d of n such that gcd(d, n/d) is an
 *          exponentially odd number (A268335).
 * https://oeis.org/A365173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative function with a(p^e) = * floor((e+5)/4) +
 *            floor((e+6)/4) = A004524(e+5). * That is, for each prime power
 *            factor p^e of n, * multiply the value floor((e+5)/4) +
 *            floor((e+6)/4). * Implementation uses trial division with a
 *            cached prime table * and a small integer square‑root ro...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root: largest r such that r*r <= n. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0;
    for (int i = 63; i >= 0; --i) {
        uint64_t y = r | ((uint64_t)1 << i);
        if (y <= n / y) r = y;          /* avoid 64-bit overflow */
    }
    return r;
}

/* Cached list of primes up to prime_lim (inclusive). */
static uint32_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static size_t   prime_lim  = 0;

/* Build all primes up to 'limit' using a classic sieve.  If 'limit'
 * is smaller than the current prime_lim, the existing list is kept.
 */
static void ensure_primes_upto(size_t limit)
{
    if (limit <= prime_lim) return;   /* already sufficient */

    /* Free old table if any. */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
        prime_cnt  = 0;
    }

    /* Sieve. */
    size_t sz = limit + 1;
    bool *is_comp = (bool *)malloc(sz * sizeof(bool));
    if (!is_comp) return;
    for (size_t i = 0; i < sz; ++i) is_comp[i] = false;

    size_t r = (size_t)isqrt_u64(limit);
    for (size_t i = 2; i <= r; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    /* Count primes. */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    /* Store primes. */
    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) { free(is_comp); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) prime_list[idx++] = (uint32_t)i;

    prime_cnt = cnt;
    prime_lim = limit;
    free(is_comp);
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety, offset is 1 */

    uint64_t m = (uint64_t)n;
    if (m == 1) return 1;           /* a(1) = 1 */

    /* Ensure we have enough primes for the largest possible factor. */
    uint64_t limit = isqrt_u64(m);
    ensure_primes_upto((size_t)limit + 1);

    seq_t result = 1;
    while (m > 1) {
        bool found = false;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > m) break;
            if (m % p == 0) {
                size_t exp = 0;
                while (m % p == 0) {
                    m /= p;
                    ++exp;
                }
                seq_t term = (seq_t)((exp + 5) / 4 + (exp + 6) / 4);
                result *= term;
                found = true;
                break;                 /* move on to next factor */
            }
        }
        if (!found) {                 /* remaining m is prime */
            result *= (seq_t)2;        /* exponent 1 => term 2 */
            break;
        }
    }
    return result;
}
