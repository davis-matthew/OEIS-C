/*
 * A354144  Prime powers together with semiprimes.
 * https://oeis.org/A354144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n-th smallest integer that is either a prime power or a
 *            semiprime
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=2, a(3)=3, … (union of A000961 and A001358) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_checked = 0;

/* smallest prime factor sieve */
static uint64_t spf_limit = 0;
static uint32_t *spf = NULL;

/* build spf array up to 'limit' (inclusive) */
static void build_spf(uint64_t limit)
{
    uint32_t *tmp = (uint32_t *)realloc(spf, sizeof(uint32_t) * (limit + 1));
    spf = tmp;
    for (uint64_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

/* ensure that spf array is large enough to contain 'min_limit' */
static void ensure_spf(uint64_t min_limit)
{
    if (min_limit <= spf_limit) return;
    uint64_t new_limit = spf_limit ? spf_limit : 1;
    while (new_limit < min_limit) new_limit <<= 1;
    build_spf(new_limit);
}

/* true if x is a prime power */
static bool is_prime_power(uint64_t x)
{
    if (x < 2) return false;
    uint64_t p = spf[x];
    if (p == 0) return false;
    uint64_t rem = x;
    while (rem % p == 0) rem /= p;
    return rem == 1;
}

/* true if x is a semiprime (exactly two prime factors with multiplicity) */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;
    uint64_t rem = x;
    uint64_t cnt = 0;
    while (rem > 1 && cnt <= 2) {
        uint64_t p = spf[rem];
        if (p == 0) return false;
        int exp = 0;
        while (rem % p == 0) {
            rem /= p;
            ++exp;
        }
        cnt += exp;
        if (cnt > 2) return false;
    }
    return cnt == 2;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, unused */

    if ((size_t)n <= cache_len)    /* already computed */
        return cache[n - 1];

    uint64_t cur;
    for (cur = last_checked + 1; cache_len < (size_t)n; ++cur) {
        ensure_spf(cur);
        if (cur == 1 || is_prime_power(cur) || is_semiprime(cur)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur;
        }
    }
    last_checked = cur - 1;
    return cache[n - 1];
}
