/*
 * A336591  Numbers whose exponents in their prime factorization are either 1,
 *          3, or both.
 * https://oeis.org/A336591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A336591.
   a(n) is the nth positive integer whose prime factorisation
   contains only exponents 1 or 3 (or none for 1).  The function
   generates terms sequentially and caches them so that any
   order of calls is handled efficiently.   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Test whether all prime exponents of a 64‑bit integer are 1 or 3.
   Returns true if true, false otherwise. */
static bool good_uint64(uint64_t x)
{
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt;

    if (x <= 1)
        return true;              /* 1 and 0 have no prime exponents */

    factor_u64(x, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] != 1 && exps[i] != 3)
            return false;
    }
    return true;
}

/* External function required by the build framework. */
seq_t a(idx_t n)
{
    /* Static cache holding all computed terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* current allocation size */
    static uint64_t candidate = 2;  /* next integer to test after 1 */

    if (n == 0)
        return 0;                     /* not used by OEIS, safe fallback */

    /* Grow the cache until at least n terms are stored. */
    while (cache_len < n) {
        /* Handle the first term (1) explicitly. */
        if (cache_len == 0) {
            if (cache_cap == 0) {
                cache_cap = 4;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
                if (!cache) return 0; /* allocation failure */
            }
            cache[0] = (seq_t)1;  /* 1 has no prime exponents */
            cache_len = 1;
            continue;            /* next iteration continues at candidate = 2 */
        }

        /* Find the next number whose prime exponents are 1 or 3. */
        while (!good_uint64(candidate))
            ++candidate;

        /* Grow the cache array if needed. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            /* Reallocate: use malloc/realloc for both C and C++ builds.
               For C++ builds seq_t is assumed to be trivially constructible. */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the valid candidate. */
        cache[cache_len] = (seq_t)candidate;
        ++cache_len;
        ++candidate;
    }

    return cache[n - 1];
}
