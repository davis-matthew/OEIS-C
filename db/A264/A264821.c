/*
 * A264821  Centered 14-gonal (or tetradecagonal) primes.
 * https://oeis.org/A264821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime of the form 7*k^2 - 7*k + 1 (k ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts with 43, 211, 631, 1093, ... */

#include <stdbool.h>
#include <stddef.h>

#include <stdint.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* Static cache of found primes.
       cache holds the primes in order; len = number of cached primes. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next_k = 3;   /* next k to test (k=3 gives the first prime) */

    /* We assume n >= 1; behaviour for n < 1 is unspecified. */

    /* Ensure the cache contains at least n primes. */
    while (len < (size_t)n) {
        /* Grow cache if necessary. */
        ensure_capacity(len + 1, &cap, OEISC_VPP(&cache), sizeof(seq_t));

        /* Compute the candidate prime value. */
        seq_t k_s = (seq_t)next_k;
        seq_t val = 7 * k_s * k_s - 7 * k_s + 1;

        /* Test primality using the provided helper. */
        if (isPrimeGeneric(val)) {
            cache[len++] = val;   /* store if prime */
        }

        ++next_k;                /* try next k in the next iteration */
    }

    /* Return the nth term (1‑based index). */
    return cache[n - 1];
}
