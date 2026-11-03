/*
 * A330600  Primes p such that the sum of (p mod d) for nonzero digits d of p
 *          is prime.
 * https://oeis.org/A330600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime p such that the sum of (p mod d) over all
 *            non‑zero decimal digits d of p is also prime. The sequence is
 *            generated on demand and memoised
 * Range:     u8 n <= 26, u16 n <= 2492, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9301 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
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

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cached = NULL;
    static size_t cached_len = 0;   /* number of terms stored */
    static size_t cached_cap = 0;   /* allocated capacity */
    /* static next candidate to test for primality */
    static seq_t next_candidate = 2;

    /* initialise cache */
    if (cached_cap == 0) {
        /* initial capacity 16 */
        ensure_capacity(16, &cached_cap, OEISC_VPP(&cached), sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cached_len < (size_t)n) {
        /* find next prime candidate */
        seq_t p;
        for (p = next_candidate; ; ++p) {
            if (isPrimeGeneric(p))
                break;
        }

        /* compute sum of (p mod d) for each non‑zero digit d of p */
        seq_t sum = 0;
        seq_t tmp = p;
        while (tmp > (seq_t)0) {
            seq_t d = tmp % (seq_t)10;
            if (d != (seq_t)0)
                sum += p % d;
            tmp /= (seq_t)10;
        }

        /* if the sum is prime, store p */
        if (isPrimeGeneric(sum)) {
            ensure_capacity(cached_len + 1, &cached_cap, OEISC_VPP(&cached), sizeof(seq_t));
            cached[cached_len++] = p;
        }

        /* advance candidate for next iteration */
        next_candidate = p + (seq_t)1;
    }

    return cached[n - 1];
}
