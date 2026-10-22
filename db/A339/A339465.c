/*
 * A339465  Primes p such that (p-1)/gpf(p-1) = 2^q * 3^r with q, r >= 1,
 *          where gpf(m) is the greatest prime factor of m, A006530.
 * https://oeis.org/A339465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   ensure_capacity, gpf, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A339465: primes p such that (p-1)/gpf(p-1) = 2^q * 3^r,  q,r ≥ 1   */
/* The sequence is generated lazily and cached.  The first stored term is
   a(1) = 19.  Subsequent calls recompute only as far as needed.      */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/gpf.h"            /* uint64_t gpf(uint64_t x) */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */
#include "../utils/ensure_capacity.h"/* void ensure_capacity(size_t, size_t*, void**, size_t) */

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
    ensure_capacity(need, &cache_cap, (void**)&cache, sizeof(seq_t));
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

        uint64_t g = gpf(candidate - 1U); /* Greatest prime factor of p-1. */
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
