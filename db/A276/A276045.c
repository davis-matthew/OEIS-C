/*
 * A276045  Primes p such that d(p*(2p+1)) = 8 where d(n) is the number of
 *          divisors of n (A000005).
 * https://oeis.org/A276045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that d(p*(2p+1)) = 8
 * Range:     u8 n <= 19, u16 n <= 1956, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9641 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

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

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Memoisation state */
static seq_t *cache = NULL;        /* array of found primes */
static idx_t cache_len = 0;        /* number of primes stored */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static uint64_t last_checked = 1; /* last integer examined */

/* Ensure the cache can hold at least 'needed' elements */
static void _grow_cache(idx_t needed)
{
    if (cache_len >= needed) return;
    if (needed > cache_cap) {
        /* Grow to at least double the current capacity or the needed size */
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    }
}

/* Generate primes until we have at least n terms in cache */
static void _ensure_cache(idx_t n)
{
    uint64_t candidate;
    for (candidate = last_checked + 1; cache_len < n; ++candidate) {
        /* Skip even numbers after 2 */
        if (candidate > 2 && (candidate & 1ULL) == 0) continue;

        /* Test primality */
        if (!is_prime_big((seq_t)candidate)) continue;

        /* 2p+1 must have exactly 4 divisors */
        uint64_t m = 2 * candidate + 1U;
        if (num_divisors(m) == 4) {
            /* Store the prime */
            _grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }
        /* Remember where we left off */
        last_checked = candidate;
    }
    /* Adjust last_checked for the next call */
    last_checked = candidate - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* offset starts at 1 */
    if (n > cache_len) _ensure_cache(n);
    return cache[n - 1];
}
