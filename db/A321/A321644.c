/*
 * A321644  Squarefree odd composite numbers whose factors are all twin primes
 *          (not necessarily from the same pair).
 * https://oeis.org/A321644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 6416, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8695 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate odd composite square‑free numbers whose prime factors are all twin primes */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
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

static seq_t *cache = NULL;      /* cache of found terms */
static size_t cache_len = 0;
static size_t cache_cap = 0;

static uint64_t *prime_list = NULL;  /* primes for trial division */
static size_t prime_cnt = 0;
static const idx_t PRIME_LIMIT = 2000000;   /* initial sieve limit */

static seq_t next_candidate = (seq_t)3;     /* first odd candidate */

/* Build prime list if not already built */
static void ensure_primes(void) {
    if (prime_list) return;
    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);
}

/* Check whether a small prime p is a twin prime */
static inline bool twin_prime_uint64(uint64_t p) {
    if (p < 3) return false;
    return is_prime_big((seq_t)(p - 2)) || is_prime_big((seq_t)(p + 2));
}

/* Append a value to the cache, growing it if necessary */
static void append_cache(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Test a candidate number */
static bool qualifies(seq_t x) {
    /* Must be odd */
    if ((x & (seq_t)1) == (seq_t)0) return false;

    /* Must be composite */
    if (is_prime_big(x)) return false;

    seq_t tmp = x;
    size_t distinct = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        seq_t pseq = (seq_t)p;

        seq_t sq = pseq * pseq;
        if (sq > tmp) break;

        if (tmp % pseq == (seq_t)0) {
            /* Count multiplicity */
            size_t cnt = 0;
            while (tmp % pseq == (seq_t)0) {
                tmp /= pseq;
                ++cnt;
                if (cnt > 1) return false; /* not squarefree */
            }
            ++distinct;
            if (!twin_prime_uint64(p)) return false;
        }
    }

    if (tmp > (seq_t)1) { /* remaining prime factor */
        /* tmp is prime (since we divided all smaller primes) */
        if (!is_prime_big(tmp)) return false;
        if (!twin_prime_uint64((uint64_t)tmp)) return false;
        ++distinct;
    }

    return distinct >= 2;  /* at least two distinct twin‑prime factors */
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined for n==0 */

    ensure_primes();

    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate;
        next_candidate += (seq_t)2;  /* stay odd */

        if (qualifies(cand)) {
            append_cache(cand);
        }
    }

    return cache[n - 1];
}
