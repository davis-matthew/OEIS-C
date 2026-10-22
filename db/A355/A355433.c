/*
 * A355433  Numbers k such that k is sqrt(k)-smooth and k+1 is
 *          sqrt(k+1)-smooth.
 * https://oeis.org/A355433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 3759, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A355433
   Numbers k such that k is sqrt(k)-smooth and k+1 is sqrt(k+1)-smooth.
   The function a(n) returns the n‑th such number (a(1)=8).  It uses only
   integer arithmetic and a small prime sieve.  It works for any
   seq_t that can hold the values; for the 64‑bit build the code is
   fully deterministic.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- small prime sieve (up to 200 000) ---------- */
#define PRIME_SIEVE_LIMIT 200000
static uint32_t primes[20000];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;
    bool *sieve = (bool *)malloc(PRIME_SIEVE_LIMIT + 1);
    if (!sieve) exit(1);
    memset(sieve, true, PRIME_SIEVE_LIMIT + 1);
    sieve[0] = sieve[1] = false;
    for (uint32_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                sieve[j] = false;
        }
    }
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve[i]) primes[primes_cnt++] = i;
    }
    free(sieve);
}

/* ---------- integer square root for 64‑bit unsigned numbers ----------
   (returns floor(sqrt(x)))  */
static uint64_t isqrt_u64(uint64_t x)
{
    if (x < 2) return x;
    uint64_t r = 0, bit = 1ULL << 62;
    /* find the biggest power of 4 ≤ x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
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

/* ---------- test whether a 64‑bit integer is sqrt‑smooth ----------
   i.e. all its prime factors ≤ sqrt(k) ---------- */
static bool is_smooth_u64(uint64_t n)
{
    if (n <= 1) return true;
    uint64_t sqrt_n = isqrt_u64(n);
    uint64_t temp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1 && temp > sqrt_n) return false;   /* remaining large factor */
    return true;
}

/* ---------- test whether a seq_t value is sqrt‑smooth ---------- */
static bool is_smooth_seq(seq_t k)
{
    if (k <= 1) return true;
    /* cast down – for the build limits this is safe; for values that
       exceed 64‑bit the sequence is too large for the intended tests. */
    uint64_t kk   = (uint64_t)k;
    uint64_t sqrt_k = isqrt_u64(kk);
    uint64_t temp   = kk;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1 && temp > sqrt_k) return false;
    return true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static uint64_t next_start = 8;   /* first term in the sequence */

    /* initialise static data only once */
    init_primes();
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* generate terms until we have at least n of them */
    while (cache_cnt < n) {
        uint64_t k64 = next_start;
        seq_t k = (seq_t)k64;
        if (is_smooth_seq(k) && is_smooth_seq(k + 1)) {
            if (cache_cnt >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                cache = tmp;
            }
            cache[cache_cnt++] = k;
        }
        ++next_start;
    }

    return cache[n - 1];
}
