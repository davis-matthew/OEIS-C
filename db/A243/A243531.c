/*
 * A243531  Prime numbers containing the string 15.
 * https://oeis.org/A243531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 252, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 907 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes whose decimal representation contains the substring "15". */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Return true if x contains the decimal substring "15". */
static bool contains_15(seq_t x)
{
    /* Check every overlapping pair of digits. */
    while (x >= 10) {
        if (x % 100 == 15)
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static state: cached primes containing "15" and search progress. */
    static seq_t *cache = NULL;          /* array of primes containing "15" */
    static idx_t count = 0;              /* number of primes stored */
    static idx_t cap   = 0;              /* current capacity of the cache */
    static seq_t cur   = 1;              /* last tested integer */

    /* Grow the cache until it contains at least n elements. */
    while (count < n) {
        /* Advance to the next candidate, skipping even integers > 2. */
        do {
            cur++;
        } while (cur % 2 == 0 && cur != 2);

        /* Test the candidate. */
        if (contains_15(cur) && isPrimeGeneric(cur)) {
            /* Ensure capacity for a new element. */
            ensure_capacity((size_t)(count + 1), &cap,
                            OEISC_VPP(&cache), sizeof(seq_t));
            cache[count++] = cur;
        }
    }
    return cache[n - 1];
}
