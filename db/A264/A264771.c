/*
 * A264771  Primes of the form n^2 + phi(n).
 * https://oeis.org/A264771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that can be written as k^2 + φ(k), where k
 *            = 1, 2, 3, … and we take the primes in the natural order of k
 * Range:     u8 n <= 7, u16 n <= 37, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/totient_seq.h"
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

static seq_t *prime_cache = NULL;     /* cached prime values   */
static size_t cache_cap   = 0;        /* allocated cache size  */
static size_t cache_len   = 0;        /* number of cached primes */
static idx_t   last_k     = 0;        /* last integer k processed */

seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if (n <= cache_len)
        return prime_cache[n - 1];

    /* Generate new primes until we have n terms. */
    while (cache_len < n) {
        ++last_k;                                   /* next k */
        seq_t k_sq   = (seq_t)last_k * (seq_t)last_k;
        seq_t phi_k  = totient_seq((seq_t)last_k);
        seq_t cand   = k_sq + phi_k;

        if (isPrimeGeneric(cand)) {
            /* Grow cache if needed. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            OEISC_VPP(&prime_cache), sizeof(seq_t));
            prime_cache[cache_len++] = cand;
        }
    }
    return prime_cache[n - 1];
}
