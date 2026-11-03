/*
 * A296012  Primes of the form k + k+1 + k+2 +-1 where k, k+1, and k+2 are all
 *          composite numbers.
 * https://oeis.org/A296012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 4070, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9940 (2026-11-03)
 * Helpers:   is_prime, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A296012 (Primes of the form k+k+1+k+2±1
   where k, k+1, k+2 are all composite).  The implementation
   incrementally searches for suitable k and collects the primes
   that satisfy the condition.  The result is cached so that
   subsequent calls in any order are efficient. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime.h"
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

static seq_t *cache = NULL;      // Array of found primes
static size_t cache_len = 0;     // Current number of primes stored
static size_t cache_cap = 0;     // Current capacity of array
static uint64_t processed_k = 0; // Last k value processed

/* Append a prime to the cache, resizing as necessary */
static void
add_prime(seq_t p)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
        ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
    }
    cache[cache_len++] = p;
}

/* Extend the cache until it contains at least n elements */
static void
ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        /* Find the next k such that k and k+2 are composite */
        do
        {
            ++processed_k;
        } while (is_prime(processed_k) || is_prime(processed_k + 2));

        /* Candidates p1 = 3*k + 2 and p2 = 3*k + 4 */
        seq_t p1 = (seq_t)processed_k * (seq_t)3 + (seq_t)2;
        seq_t p2 = p1 + (seq_t)2;

        if (isPrimeGeneric(p1))
            add_prime(p1);
        if (isPrimeGeneric(p2))
            add_prime(p2);
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          // Undefined below offset; harmless
    ensure_cache(n);
    return cache[n - 1];          // a(1) is the first element
}
