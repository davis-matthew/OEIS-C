/*
 * A278922  Largest p such that n = p + q + r where p < q < r are all prime,
 *          or 0 if no such primes p, q, r exist.
 * https://oeis.org/A278922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest p such that n = p + q + r with p < q < r all
 *            primes, or 0 if no such primes exist
 * Range:     u8 n <= 788, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 137 terms through n = 9809 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps a cached list of primes up to the maximum n
   seen so far. For each n it works out whether n is even or odd:
   - Even n: one of the primes must be 2; we simply test q + r = n - 2.
   - Odd n: 2 cannot be used. We search the primes in descending order
     to guarantee the largest possible p is found first.  */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Pure prime table builder up to a given limit.  The table is freed
   and rebuilt when a larger limit is required. */
static void build_primes_upto_limit(idx_t limit, uint64_t **primes_out,
                                    size_t *cnt_out)
{
    size_t cap = 16;
    /* Allocate the prime array. */
    uint64_t *pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t cnt = 0;

    /* Sieve of Eratosthenes (byte array of composites). */
    bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            if (cnt >= cap) {
                cap <<= 1;
                pr = (uint64_t *)realloc(pr, cap * sizeof(uint64_t));
            }
            pr[cnt++] = (uint64_t)i;
            if ((idx_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    free(is_comp);
    *primes_out = pr;
    *cnt_out    = cnt;
}

/* Helper testing whether x (<= current limit) is prime using the table. */
static bool is_prime_x(uint64_t x, uint64_t *primes, size_t prime_cnt)
{
    if (x < 2U) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if ((unsigned __int128)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    /* Persistent prime table across calls. */
    static uint64_t *primes      = NULL;
    static size_t   prime_cnt     = 0;
    static idx_t    primes_limit  = 0;

    /* Grow enough primes if necessary. */
    if (primes == NULL || n > primes_limit) {
        if (primes) free(primes);
        build_primes_upto_limit(n, &primes, &prime_cnt);
        primes_limit = n;
    }

    /* For very small n the answer is always 0. */
    if (n < 10U) return (seq_t)0;

    /* Even n: one prime must be 2. */
    if ((n & 1U) == 0U) {
        idx_t sum = n - 2U;                 /* need q + r = sum, q < r. */
        for (size_t i = 0; i < prime_cnt && primes[i] < sum / 2U; ++i) {
            uint64_t q = primes[i];
            uint64_t r = sum - q;
            if (r <= q) break;              /* q < r required. */
            if (is_prime_x(r, primes, prime_cnt))
                return (seq_t)2U;           /* only possible p is 2. */
        }
        return (seq_t)0;
    }

    /* Odd n: 2 cannot be part of the triple. Search for the largest p. */
    for (size_t idx = prime_cnt; idx-- > 0; ) {
        uint64_t p = primes[idx];
        if (p <= 2U) continue;          /* skip 2; p must be an odd prime. */

        /* Quick bound: smallest possible sum with this p is p + 5 + 7. */
        if (p > n - 12U) continue;

        idx_t remaining = n - p;        /* need q + r = remaining, q < r. */

        /* Find the first prime > p as starting point for q. */
        size_t start = idx + 1;
        for (size_t j = start; j < prime_cnt && primes[j] < remaining / 2U; ++j) {
            uint64_t q = primes[j];
            uint64_t r = remaining - q;
            if (r <= q) break;          /* q < r required. */
            if (is_prime_x(r, primes, prime_cnt))
                return (seq_t)p;        /* largest p found. */
        }
    }

    /* No appropriate triple found. */
    return (seq_t)0;
}
