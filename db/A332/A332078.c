/*
 * A332078  Primes p = k*2^m + 1 such that k + 2^m is not prime, where k and m
 *          are the odd part and 2-valuation, respectively, of p-1.
 * https://oeis.org/A332078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p = k*2^m + 1 where k is the odd part of
 *            p‑1, * m = valuation2(p‑1), and k + 2^m is composite
 *            (non‑prime). This is OEIS A332078
 * Range:     u8 n <= 12, u16 n <= 4164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8976 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Utility prototypes (provided by the build system). */
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"
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

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t cache_len = 0;      /* number of terms currently stored */

static idx_t last_checked = 2;   /* last integer examined for primality */

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Find the next prime greater than last_checked. */
        idx_t cand = last_checked + 1;
        while (!is_prime(cand))
            cand++;
        last_checked = cand;

        /* Compute m (valuation of 2 in p-1) and k (odd part of p-1). */
        idx_t temp = cand - 1;
        idx_t m = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            m++;
        }
        seq_t k = (seq_t)temp;          /* odd part of p-1 */
        seq_t pow2m = pow2(m);          /* 2^m */
        seq_t s = k + pow2m;            /* k + 2^m */

        /* If s is composite, store p in the cache. */
        if (!is_prime_big(s)) {
            ensure_capacity(cache_len + 1, &cache_cap,
                            OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = (seq_t)cand;
        }
    }
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
