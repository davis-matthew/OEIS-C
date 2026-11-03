/*
 * A352272  Numbers whose squarefree part is congruent to 1 modulo 6.
 * https://oeis.org/A352272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352272 – Numbers whose squarefree part is ≡ 1 (mod 6).
 * The function scans integers, tests the condition by trial
 * division up to the square root, and memoises the results to
 * answer any n >= 1 in O(n) time (negligible overhead for n ≤ 10000).
 *
 * The implementation works for all supported seq_t types:
 *  - ordinary arithmetic types (uint8_t … uint64_t)
 *  - an arbitrary‑precision integer class with overloaded
 *    + − * / % and comparison operators.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions provided by the framework */
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/*  Test whether the squarefree part of x is congruent to 1 (mod 6). */
static bool good_squarefree_mod6(seq_t x)
{
    seq_t tmp = x;
    int mod = 1;                     /* product of odd‑exponent primes (mod 6) */

    /* trial division by all integers up to √tmp */
    for (int p = 2; ((seq_t)p * (seq_t)p) <= tmp; ++p) {
        seq_t P = (seq_t)p;
        if (tmp % P == 0) {
            int cnt = 0;
            while (tmp % P == 0) {
                tmp /= P;
                ++cnt;
            }
            if (cnt & 1) {                /* odd exponent */
                if (p == 2 || p == 3)     /* 2 or 3 would give squarefree part ≠ 1 mod 6 */
                    return false;
                mod = (mod * (p % 6)) % 6; /* multiply the prime’s residue */
            }
        }
    }

    /* any remaining prime factor (must be > √original) */
    if (tmp > (seq_t)1) {
        seq_t P = tmp;                 /* now P is a prime > √original */
        if (P == (seq_t)2 || P == (seq_t)3)
            return false;
        /* P appears with exponent 1 (odd) */
        uint64_t rem = (uint64_t)P;    /* fits into 64‑bit for values in the test range */
        mod = (mod * (int)(rem % 6)) % 6;
    }

    return mod == 1;
}

/* --------------------------------------------------------------- */

/* --------------------------------------------------------------- */
/*  OEIS sequence A352272.  External linkage – exactly the function
 *  required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety – the sequence is 1‑indexed */

    static seq_t *cache = NULL;        /* memoised values */
    static size_t cache_len = 0;       /* number of stored terms */
    static size_t cache_cap = 0;       /* allocated capacity */
    static seq_t next_val = (seq_t)1;  /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Find the next qualifying integer */
        while (!good_squarefree_mod6(next_val))
            ++next_val;

        /* Store the new term */
        if (cache_len == cache_cap) {           /* grow the cache */
            size_t new_cap = cache_cap ? cache_cap << 1 : 4;
            seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_len] = next_val;
        ++cache_len;
        ++next_val;   /* advance for the next candidate */
    }

    return cache[n - 1];
}
