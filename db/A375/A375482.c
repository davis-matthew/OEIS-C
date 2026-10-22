/*
 * A375482  a(n) is the number of points (k, prime(k)) at integer distance
 *          from the point (n, prime(n)) for k < n (where prime(k) denotes the
 *          k-th prime number).
 * https://oeis.org/A375482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of k < n such that the Euclidean distance
 *            between (k,prime(k)) and (n,prime(n)) is an integer
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Integer square root of a 64‑bit value (floor). */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0, bit = 0x4000000000000000ULL;   /* 2^(62) */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Prime list: stored as 64‑bit unsigned integers. */
static uint64_t *prime_list   = NULL;
static size_t   prime_cap    = 0;
static size_t   prime_count  = 0;

/* Ensure that prime_list holds the first n primes (n >= 1). */
static void generate_primes_to(idx_t n)
{
    if (prime_count >= (size_t)n) return;

    if (prime_cap < (size_t)n) {
        size_t new_cap = prime_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) return;          /* fail silently – not expected for n <= 10000 */
        prime_list = tmp;
        prime_cap = new_cap;
    }

    uint64_t cand = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
    while (prime_count < (size_t)n) {
        bool isprime = true;
        uint64_t sq = isqrt_u64(cand);
        for (size_t i = 0; i < prime_count && prime_list[i] <= sq; ++i) {
            if (cand % prime_list[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            prime_list[prime_count++] = cand;
        }
        ++cand;
    }
}

/* Cache of previously computed a(n) values. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static idx_t   cache_len = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n < 1 */

    /* Ensure enough primes are available. */
    generate_primes_to(n);

    /* Grow cache if necessary and compute missing entries. */
    if (n > cache_len) {
        size_t new_cap = cache_cap ? cache_cap : n;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* out of memory – unlikely for n <= 10000 */
        cache = tmp;
        cache_cap = new_cap;

        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t cnt = 0;
            uint64_t pi = prime_list[(size_t)i - 1];  /* prime(i) */
            for (idx_t k = 1; k < i; ++k) {
                uint64_t pk = prime_list[(size_t)k - 1];
                uint64_t dx = (uint64_t)(i - k);
                uint64_t dy = pi - pk;
                uint64_t d2 = dx * dx + dy * dy;
                uint64_t s  = isqrt_u64(d2);
                if (s * s == d2) ++cnt;
            }
            cache[(size_t)i - 1] = cnt;
        }
        cache_len = n;
    }

    return cache[(size_t)n - 1];
}
