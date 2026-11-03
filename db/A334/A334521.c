/*
 * A334521  Numbers that are not congruent to +- mod 9 and are neither a cube
 *          nor twice a cube.
 * https://oeis.org/A334521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A334521: Numbers not congruent to 4 or 5 (mod 9),
   and not a cube nor twice a cube.  */
/* The function returns the n‑th term (a(1)=3). */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache  = NULL;        /* pointer to the cache array */
    static size_t cache_len = 0;        /* number of terms stored */
    static size_t cache_cap = 0;        /* current capacity of the cache */
    static seq_t next_val = 1;          /* next integer to test */

    if (n == 0)          /* the sequence is offset 1; index 0 never used */
        return 0;

    /* grow the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        seq_t val = next_val;

        /* skip numbers congruent to 4 or 5 modulo 9 */
        seq_t mod9 = val % 9;
        if (mod9 == 4 || mod9 == 5)  { next_val++; continue; }

        /* skip perfect cubes */
        seq_t croot = floor_cube_root(val);
        if (croot * croot * croot == val)  { next_val++; continue; }

        /* skip numbers equal to twice a perfect cube */
        if ((val & 1) == 0) {
            seq_t half   = val / 2;
            seq_t croot2 = floor_cube_root(half);
            if (croot2 * croot2 * croot2 * 2 == val) { next_val++; continue; }
        }

        /* the number satisfies all conditions – store it */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            /* use realloc that is safe for both C and C++ (seq_t may be a class) */
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return 0;   /* allocation failure – abort */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = val;
        next_val++;
    }

    return cache[n - 1];
}
