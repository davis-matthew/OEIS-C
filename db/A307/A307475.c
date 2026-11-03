/*
 * A307475  SanD-86 primes: primes p such that p+d is also prime and sum of
 *          digits A007953(p(p+d)) = d, with d = 86.
 * https://oeis.org/A307475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th SanD‑86 prime – a prime p such that
 *   1) p is prime,
 *   2) p+86 is prime,
 *   3) digit_sum(p * (p+86)) == 86.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

#ifdef __cplusplus
#include <vector>
/* C++ implementation – uses std::vector for storage */
static std::vector<seq_t> cache;
static uint64_t next_candidate = 3;

seq_t a(idx_t n) {
    while (cache.size() < (size_t)n) {
        uint64_t p = next_candidate;
        p = next_prime(p);              /* smallest prime >= p */
        next_candidate = p + 1;         /* advance for next search */

        uint64_t q = p + 86;
        if (!is_prime_big((seq_t)q))    /* p+86 must be prime */
            continue;

        seq_t prod = (seq_t)p * (seq_t)q;           /* p * (p+86) */
        if (digit_sum(prod) != (seq_t)86)
            continue;

        cache.push_back((seq_t)p);          /* record SanD‑86 prime */
    }
    return cache[(size_t)n - 1];            /* 1‑based indexing */
}
#else
/* C implementation – uses malloc/realloc for storage */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 3;

static void ensure_capacity(size_t needed) {
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – terminate */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;
        p = next_prime(p);              /* smallest prime >= p */
        next_candidate = p + 1;         /* advance for next search */

        uint64_t q = p + 86;
        if (!is_prime_big((seq_t)q))    /* p+86 must be prime */
            continue;

        seq_t prod = (seq_t)p * (seq_t)q;           /* p * (p+86) */
        if (digit_sum(prod) != (seq_t)86)
            continue;

        ensure_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)p;             /* record SanD‑86 prime */
    }
    return cache[(size_t)n - 1];
}
#endif
