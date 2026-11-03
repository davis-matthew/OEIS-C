/*
 * A386536  Exponent of the highest power of 2 dividing the n-th number that
 *          is cubefree but not squarefree.
 * https://oeis.org/A386536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9781 (2026-11-03)
 * Helpers:   ctz_idx_to_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A386536: exponent of 2 dividing the n‑th integer that is cubefree but not squarefree.
   It is computed by a linear search over the natural numbers, caching results so that
   repeated calls are fast.  No closed form is known. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ctz_idx_to_seq.h"
#include "../utils/mem.h"

/* Test whether x is cubefree but not squarefree */
static bool is_cubefree_not_squarefree(idx_t x)
{
    bool has_square = false;
    idx_t temp = x;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
                if (e == 3)          /* exponent 3 → not cubefree */
                    return false;
            }
            if (e == 2)
                has_square = true;
        }
    }
    /* Remaining prime factor (if any) has exponent 1 */
    return has_square;
}

/* --------------------------------------------------------------------- */
/* Cache for computed values                                         */
/* --------------------------------------------------------------------- */
static seq_t *cache   = NULL;   /* array of a(k) */
static size_t cache_cnt = 0;    /* number of stored values */
static size_t cache_cap = 0;    /* allocated capacity */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Ensure cache has room for at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Next candidate integer to test */
static idx_t next_to_check = 1;

/* --------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n==0 but avoid UB */
        return 0;

    /* Extend cache until we have at least n entries */
    while ((size_t)n > cache_cnt) {
        if (is_cubefree_not_squarefree(next_to_check)) {
            ensure_cache_cap(cache_cnt + 1);
            cache[cache_cnt++] = ctz_idx_to_seq(next_to_check);
        }
        ++next_to_check;
    }
    return cache[n - 1];
}
