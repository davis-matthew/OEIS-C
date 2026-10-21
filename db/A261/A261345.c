/*
 * A261345  Number of distinct prime divisors among the numbers k^2 + 1 for k
 *          in 1 <= k <= n.
 * https://oeis.org/A261345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime divisors among k^2 + 1 for 1 ≤ k
 *            ≤ n
 * Range:     u8 n <= 358, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --------- Basic utilities --------- */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((__uint128_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* --------- Prime generation (trial division) --------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0, prime_cap = 0;
static uint64_t prime_limit_gen = 1;

static void append_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_cnt++] = p;
}

static void ensure_prime_limit(uint64_t limit)
{
    if (prime_limit_gen >= limit) return;
    for (uint64_t cand = prime_limit_gen + 1; cand <= limit; ++cand) {
        bool is_p = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_p = false; break; }
        }
        if (is_p) append_prime(cand);
    }
    prime_limit_gen = limit;
}

/* --------- Unique prime set (sorted) --------- */
static uint64_t *unique_primes = NULL;
static size_t unique_cnt = 0, unique_cap = 0;

static void add_unique_prime(uint64_t p)
{
    size_t lo = 0, hi = unique_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (unique_primes[mid] == p) return;
        if (unique_primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    if (unique_cnt == unique_cap) {
        size_t new_cap = unique_cap ? unique_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(unique_primes, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        unique_primes = tmp;
        unique_cap = new_cap;
    }
    memmove(&unique_primes[lo + 1], &unique_primes[lo],
            (unique_cnt - lo) * sizeof(uint64_t));
    unique_primes[lo] = p;
    ++unique_cnt;
}

/* --------- Sequence cache --------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_n_done = 0;

/* --------- Main function --------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Grow cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Process new k values if any */
    for (idx_t k = max_n_done + 1; k <= n; ++k) {
        uint64_t kk = (uint64_t)k;
        uint64_t m  = kk * kk + 1;      /* k^2 + 1 */

        uint64_t temp = m;
        uint64_t sqrt_m = isqrt_u64(temp);
        ensure_prime_limit(sqrt_m);

        for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i) {
            uint64_t p = prime_list[i];
            if (temp % p == 0) {
                add_unique_prime(p);
                while (temp % p == 0) temp /= p;
            }
        }
        if (temp > 1) add_unique_prime(temp);

        cache[(size_t)k] = (seq_t)unique_cnt;
    }

    max_n_done = n;
    return cache[(size_t)n];
}
