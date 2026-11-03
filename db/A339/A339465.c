/*
 * A339465  Primes p such that (p-1)/gpf(p-1) = 2^q * 3^r with q, r >= 1,
 *          where gpf(m) is the greatest prime factor of m, A006530.
 * https://oeis.org/A339465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1290, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9039 (2026-11-03)
 * Helpers:   is_prime_u64, largest_prime_factor, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A339465: primes p such that (p-1)/gpf(p-1) = 2^q * 3^r,  q,r ≥ 1   */
/* The sequence is generated lazily and cached.  The first stored term is
   a(1) = 19.  Subsequent calls recompute only as far as needed.      */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/largest_prime_factor.h"
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

/* Cache storage */
static seq_t*  cache  = NULL;   /* array of computed terms   */
static size_t cache_len = 0;    /* number of stored terms    */
static size_t cache_cap = 0;    /* current allocated capacity */

/* Next integer to test for primality */
static uint64_t candidate = 2U;

/* ------------------------------------------------------------ */
/* Ensure that the cache array has at least |need| elements.   */
/* ------------------------------------------------------------ */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    /* Let ensure_capacity grow the array appropriately.          */
    ensure_capacity(need, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
}

/* ------------------------------------------------------------ */
/* Append a new value to the cache.                            */
/* ------------------------------------------------------------ */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) ensure_cache_cap(cache_len + 1);
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------ */
/* Return the n-th member of A339465 (1‑based indexing).       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise the static state on the first call. */
    static bool initialized = false;
    if (!initialized) {
        candidate = 2U;
        initialized = true;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        candidate++;                      /* Move to the next integer.    */
        if (!is_prime_big((seq_t)candidate)) continue;  /* Skip composites. */

        /* Optional optimisation: primes in this sequence are of the form 6m+1. */
        if ((candidate % 6U) != 1U) continue;

        uint64_t g = largest_prime_factor(candidate - 1U); /* Greatest prime factor of p-1. */
        if (g == 0U) continue;           /* Should never happen.          */

        uint64_t m = (candidate - 1U) / g;
        /* To satisfy the definition we need m = 2^q * 3^r with q,r≥1. */
        if ((m % 2U) == 0U && (m % 3U) == 0U) {
            while ((m % 2U) == 0U) m /= 2U;   /* Strip all factors of 2. */
            while ((m % 3U) == 0U) m /= 3U;   /* Strip all factors of 3. */
            if (m == 1U)                     /* Only 2 and 3 remain.      */
                cache_append((seq_t)candidate);
        }
    }

    return cache[(size_t)n - 1];
}
