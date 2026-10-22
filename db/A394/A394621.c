/*
 * A394621  Numbers k of the form k = p1 + p2 = x^2 + y^2 + p1^2, where p1 and
 *          p2 are primes, x, y are integers.
 * https://oeis.org/A394621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth smallest integer k such that there exist primes p1,
 *            p2 and integers x, y with k = p1 + p2 = p1^2 + x^2 + y^2
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* prime array kept globally */
static uint64_t *prime_arr = NULL;
static size_t prime_cnt   = 0;
static uint64_t prime_lim = 0;

/* construct sieve of Eratosthenes up to n (inclusive) */
static void build_primes_up_to(uint64_t n)
{
    if (prime_arr) {
        free(prime_arr);
        prime_arr = NULL;
        prime_cnt = 0;
        prime_lim = 0;
    }

    uint8_t* mark = (uint8_t*)malloc((size_t)(n + 1));
    if (!mark) exit(1);
    for (uint64_t i = 0; i <= n; ++i) mark[i] = 1;
    if (n >= 0) mark[0] = 0;
    if (n >= 1) mark[1] = 0;

    uint64_t sq = (uint64_t)sqrt((long double)n);
    for (uint64_t i = 2; i <= sq; ++i) {
        if (mark[i]) {
            for (uint64_t j = i * i; j <= n; j += i) mark[j] = 0;
        }
    }

    size_t cnt = 0;
    for (uint64_t i = 2; i <= n; ++i) if (mark[i]) ++cnt;

    prime_arr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_arr) exit(1);
    size_t idx = 0;
    for (uint64_t i = 2; i <= n; ++i) if (mark[i]) prime_arr[idx++] = i;

    prime_cnt = cnt;
    prime_lim = n;
    free(mark);
}

/* make sure we have primes up to at least 'lim' */
static void ensure_primes(uint64_t lim)
{
    if (lim <= prime_lim) return;
    build_primes_up_to(lim);
}

/* trial division primality test using the global prime list */
static bool is_prime_u64(uint64_t x)
{
    if (x < 2) return false;

    uint64_t lim = (uint64_t)sqrt((long double)x) + 1;
    ensure_primes(lim);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p > x) break;
        if (p == x) return true;
        if (x % p == 0) return false;
    }
    return true;
}

/* checks if n is sum of two squares (0 allowed) using prime factor theorem */
static bool is_sum_of_two_squares_u64(uint64_t n)
{
    if (n == 0) return true;

    uint64_t lim = (uint64_t)sqrt((long double)n) + 1;
    ensure_primes(lim);

    uint64_t temp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if ((p & 3) == 3 && (exp & 1)) return false;
        }
    }
    if (temp > 1) {                /* remaining prime factor */
        if (temp % 4 == 3) return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_k = 3;   /* next k to try is 4 */

    if (n == 0) return (seq_t)0; /* offset is 1, but defined for completeness */

    /* compute terms up to requested n */
    while (cache_len < (size_t)n) {
        uint64_t k = last_k + 1;
        for (;; ++k) {
            uint64_t sq_k = (uint64_t)sqrt((long double)k);
            ensure_primes(sq_k + 10);      /* primes up to sqrt(k) needed */

            for (size_t i = 0; i < prime_cnt && prime_arr[i] <= sq_k; ++i) {
                uint64_t p1 = prime_arr[i];
                if (p1 > k - 2) continue; /* need p2 >= 2 */

                uint64_t p2 = k - p1;
                if (!is_prime_u64(p2)) continue;

                uint64_t t = k - p1 * p1;
                if (t < 0) continue;
                if (t && !is_sum_of_two_squares_u64(t)) continue;
                /* t == 0 is accepted (0 = 0^2+0^2) */

                /* store k */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)k;
                last_k = k;               /* next search starts after this k */
                break;
            }
            if (cache_len > 0 && cache[cache_len - 1] == (seq_t)k) break;
        }
    }

    return cache[n - 1];
}
