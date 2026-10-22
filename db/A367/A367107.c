/*
 * A367107  Numbers m not divisible by prime(bigomega(m)). Heinz numbers of
 *          integer partitions whose length is not a part (counted by
 *          A229816).
 * https://oeis.org/A367107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m (m ≥ 2) such that prime(bigomega(m)) *
 *            does not divide m. (bigomega counts prime factors with
 *            multiplicity)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- prime sieve for a fixed upper bound ---------- */
#define PRIME_LIMIT 100000          /* enough for n up to ~10⁵ terms */
static uint32_t primes[PRIME_LIMIT / 2 + 1];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    bool sieve[PRIME_LIMIT + 1];
    for (int i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i) sieve[j] = false;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) primes[prime_cnt++] = (uint32_t)i;
    }
    primes_ready = true;
}

/* ---------- count prime factors with multiplicity (uint64_t) ---------- */
static inline size_t bigomega_uint64(uint64_t n)
{
    size_t omega = 0;
    uint64_t x = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= x; ++i) {
        uint32_t p = primes[i];
        while (x % p == 0) {
            ++omega;
            x /= p;
        }
    }
    if (x > 1) ++omega;          /* remaining prime factor > sqrt(n) */
    return omega;
}

/* ---------- sequence generator ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_sz  = 0;     /* number of terms stored */
    static uint64_t next_m = 2;      /* next integer to test */

    if (!primes_ready) init_primes();

    /* grow cache until it contains at least n terms */
    while (cache_sz < (size_t)n) {
        uint64_t m = next_m++;
        size_t omega = bigomega_uint64(m);
        if (omega == 0) continue;           /* shouldn't happen for m ≥ 2 */
        uint32_t p = primes[omega - 1];      /* omega ≥ 1 */
        if (m % p != 0) {                   /* include m */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {                 /* allocation failed */
                    if (cache) free(cache);
                    cache = NULL;
                    cache_cap = 0;
                    cache_sz  = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
