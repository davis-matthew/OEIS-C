/*
 * A386176  Primes having only {3, 4, 7, 8} as digits.
 * https://oeis.org/A386176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9528 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence: A386176    
    Primes having only {3, 4, 7, 8} as decimal digits
*/
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
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

static seq_t *cache = NULL;      /* cached primes of the sequence     */
static size_t cache_len = 0;      /* number of elements in cache       */
static size_t cache_cap = 0;      /* capacity of cache array           */

/* Current state of the incremental generator */
static idx_t  current_len = 1;                    /* current number of digits      */
static uint64_t counter       = 0;                 /* index of the next candidate   */
static uint64_t total_counters = 4;                 /* 4^current_len                */
static unsigned char digits[32];                   /* temporary digit storage      */
static const unsigned char digit_map[4] = {3, 4, 7, 8};

/* Ensures that at least `n` terms are in the cache */
static void
ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    ensure_capacity((size_t)n, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

    while (cache_len < (size_t)n) {
        /* if counter exceeded current length, move to next length */
        if (counter >= total_counters) {
            ++current_len;
            counter = 0;
            total_counters = 1;
            for (unsigned i = 0; i < current_len; ++i)
                total_counters *= 4;           /* 4^current_len */
        }

        /* Build the candidate number from base-4 counter */
        uint64_t tmp = counter;
        for (unsigned i = 0; i < current_len; ++i) {
            unsigned idx = (unsigned)(tmp % 4);
            tmp /= 4;
            digits[i] = digit_map[idx];
        }

        /* Convert digits (least significant first) to numeric seq_t */
        seq_t x = 0;
        for (int i = (int)current_len - 1; i >= 0; --i)
            x = x * 10 + digits[i];

        /* Advance counter for next iteration */
        ++counter;

        /* Test primality and cache if prime */
        if (isPrimeGeneric(x))
            append_candidate(&cache, &cache_len, &cache_cap, x);
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based in OEIS; convert to 0‑based index in code */
    if (n == 0)
        return 0;          /* undefined, but guard against accidental call */

    ensure_cache(n);
    return cache[(size_t)n - 1];
}
