/*
 * A278425  Largest k such that there are no primes between kn and k(n+1); -1
 *          if no such k exists.
 * https://oeis.org/A278425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   lower_bound_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest k such that there are no primes between k*n and k(n+1);
   -1 if no such k exists. */
/* The function searches k=1..n, keeps the largest k with
   a zero‑prime interval, and returns it.  For the required n
   ranges this terminates quickly. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/lower_bound_uint32.h"

/* --- binary search lower bound on an array of uint32_t --- */

/* --- rebuild prime sieve and list to cover up to limit = n*(n+1) --- */
static void rebuild_primes(idx_t n, uint8_t **bits, uint32_t **primes,
                           size_t *prime_count, size_t *limit_ptr) {
    idx_t limit = n * (n + 1);
    size_t byte_len = (limit + 8) >> 3;       /* number of bytes needed */

    *bits = (uint8_t *)realloc(*bits, byte_len);
    if (!*bits) abort();
    memset(*bits, 0xFF, byte_len);           /* set all bits to 1 */

    /* clear 0 and 1 */
    if (limit >= 0) (*bits)[0] &= ~(1U << 0);
    if (limit >= 1) (*bits)[0] &= ~(1U << 1);

    /* sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!((*bits)[p >> 3] & (1U << (p & 7)))) continue;
        for (idx_t j = p * p; j <= limit; j += p)
            (*bits)[j >> 3] &= ~(1U << (j & 7));
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if ((*bits)[i >> 3] & (1U << (i & 7)))
            ++cnt;

    *primes = (uint32_t *)realloc(*primes, cnt * sizeof(uint32_t));
    if (!*primes) abort();
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if ((*bits)[i >> 3] & (1U << (i & 7)))
            (*primes)[idx++] = (uint32_t)i;

    *prime_count = cnt;
    *limit_ptr   = (size_t)limit;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)-1;   /* should not occur at offset 1 */

    /* static cache of primes for incremental build */
    static idx_t cached_n = 0;
    static uint8_t *prime_bits = NULL;
    static uint32_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static size_t sieve_limit = 0;

    if (n > cached_n) {
        rebuild_primes(n, &prime_bits, &prime_list, &prime_cnt, &sieve_limit);
        cached_n = n;
    }

    /* search k=1..n for the largest zero‑prime interval */
    seq_t last_zero = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t low  = k * n;          /* inclusive lower bound of kn */
        idx_t high = k * (n + 1);    /* exclusive upper bound of k(n+1) */

        idx_t lo = low + 1;          /* first integer strictly > low */
        idx_t hi = high - 1;         /* last integer < high */

        bool has_prime = false;
        if (lo <= hi) {
            size_t i_lo = lower_bound_uint32(prime_list, prime_cnt, (uint32_t)lo);
            size_t i_hi = lower_bound_uint32(prime_list, prime_cnt,
                                             (uint32_t)(hi + 1));
            has_prime = (i_hi > i_lo);
        }
        if (!has_prime) last_zero = k;
    }

    if (last_zero == 0)
        return (seq_t)-1;
    return (seq_t)last_zero;
}
