/*
 * A285226  Primes with integer arithmetic mean of digits = 5 in base 10.
 * https://oeis.org/A285226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Primes p such that digit_sum(p) == 5 * digit_len(p) (average
 *            digit = 5)
 * Range:     u8 n <= 4, u16 n <= 557, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 963 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u128, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size) /* elem_size: sizeof(T) */
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, new_cap);
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

/* Return the n-th prime whose integer arithmetic mean of digits equals 5.
 * The sequence is indexed from 1. */
seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = NULL;      /* array of found primes */
    static size_t cache_len = 0;     /* number of elements stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    /* Static variable to remember how far we have searched */
    static seq_t current = 2;        /* next integer to test */

    if (n == 0)  /* offset is 1; for n<1 behavior is irrelevant */
        return 0;

    /* Grow cache capacity as needed */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Keep generating primes until we have at least n terms */
    while (cache_len < (size_t)n) {
        if (isPrimeGeneric(current)) {
            seq_t sum   = digit_sum(current);
            int  digits = num_digits(current);
            if (sum == 5 * (seq_t)digits) {
                /* Append to cache */
                append_candidate(&cache, &cache_len, &cache_cap, current);
            }
        }
        current++;  /* move to next candidate */
    }

    return cache[n - 1];
}
