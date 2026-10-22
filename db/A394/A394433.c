/*
 * A394433  Numbers k such that gcd(k, phi(k)) is prime.
 * https://oeis.org/A394433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   gcd, is_prime_big, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A394433: numbers k such that gcd(k, φ(k)) is prime
   The function generates the sequence by testing successive integers
   for the required property and caches the results.  It works with
   any seq_t type (unsigned integer or arbitrary‑precision class). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#ifdef __cplusplus
#include <algorithm>
#endif

#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/is_prime_big.h"

/* static cache of found terms */
static seq_t *cache   = NULL;   /* array of seq_t values */
static size_t len     = 0;      /* number of terms stored */
static size_t cap     = 0;      /* allocated capacity of the array */
static idx_t  next_k  = 0;      /* next integer to test (1,2,3,…) */

/* Grow the cache to at least newcap entries.  Uses new[] in C++ builds
   and realloc in pure C builds to avoid calling constructors incorrectly. */
static void grow_cache(void)
{
    size_t newcap = cap ? cap * 2U : 8U;
#ifdef __cplusplus
    seq_t *tmp = new seq_t[newcap];
    if (cache) {
        std::copy(cache, cache + len, tmp);
        delete[] cache;
    }
    cache = tmp;
#else
    /* realloc returns void*; cast to seq_t* for C and C++ */
    seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    cache = tmp;
#endif
    cap = newcap;
}

/* Append a term to the cache, growing it if necessary. */
static void append_cache(seq_t v)
{
    if (len >= cap) grow_cache();
    cache[len++] = v;
}

/* Return the nth term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for 0, but return 0 */

    /* Extend cache until we have at least n terms. */
    while (len < n)
    {
        ++next_k;                      /* test k = 1,2,3,… */
        seq_t k      = (seq_t)next_k;
        seq_t phi_k  = phi(next_k);      /* φ(k) */
        seq_t g      = gcd(k, phi_k);

        /* Check that g is a prime > 1. */
        if (g > (seq_t)1 && is_prime_big(g))
            append_cache(k);
    }

    return cache[n - 1];
}
