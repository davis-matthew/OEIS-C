/*
 * A390441  Numbers whose prime factorization exponents are neither 2 nor 3.
 * https://oeis.org/A390441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 174, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose prime factorization exponents are neither 2 nor 3
   (OEIS A390441).  The sequence is calculated by testing each integer
   n = 1, 2, … and keeping the first n terms that satisfy the condition. */

/* Using only the standard C library. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Grow a dynamic array iff more space is needed. */
static void ensure_cap(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        while (new_cap < needed)
            new_cap <<= 1;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp) abort();          /* out of memory – terminate */
        *ptr   = tmp;
        *cap   = new_cap;
    }
}

/* Does the integer n satisfy the “no exponent 2 or 3” condition? */
static bool valid_candidate(uint64_t n)
{
    if (n <= 1)            /* 1 is allowed */
        return true;

    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp == 2 || exp == 3)   /* forbidden exponent */
                return false;
        }
    }
    /* Remaining factor (if any) has exponent 1 – allowed. */
    return true;
}

/* Static cache of already‑computed terms. */
static seq_t *cache      = NULL;   /* array of computed terms          */
static size_t cache_len  = 0;      /* number of terms stored           */
static size_t cache_cap  = 0;      /* allocated capacity of the array   */
static uint64_t next_candidate = 1;/* next integer to test             */

seq_t a(idx_t n)
{
    /* Ensure we have at least n terms in the cache. */
    while (cache_len < (size_t)n) {
        if (valid_candidate(next_candidate)) {
            ensure_cap(cache_len + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
