/*
 * A352272  Numbers whose squarefree part is congruent to 1 modulo 6.
 * https://oeis.org/A352272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   good_squarefree_mod6, seq_alloc_new_array, seq_realloc
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
#include "../utils/seq_alloc_new_array.h"   /* seq_t* seq_alloc_new_array(size_t n) */
#include "../utils/seq_realloc.h"           /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */
#include "../utils/good_squarefree_mod6.h"

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
