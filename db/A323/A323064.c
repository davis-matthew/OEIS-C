/*
 * A323064  Squares whose binary complement (A035327) is a cube.
 * https://oeis.org/A323064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   bit_length, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by exhaustive search: squares whose binary complement is a perfect cube. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"
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

/* Cache for the sequence. */
static seq_t *terms      = NULL;   /* array of found squares            */
static size_t terms_len  = 0;       /* number of elements in terms       */
static size_t terms_cap  = 0;       /* allocated capacity for terms      */
static idx_t  next_root  = 0;       /* next integer to test as a root    */

static inline unsigned int intbitlen(idx_t x) {
    return (x == 0) ? 1 : (unsigned int)bit_length(x);
}

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined, but avoid crash */
        return 0;

    /* Build the cache until it contains at least n terms. */
    while (terms_len < (size_t)n) {
        /* Candidate square s = next_root^2. */
        seq_t s = (seq_t)next_root * next_root;

        /* Length L of s in binary (definition of the complement). */
        unsigned int L = intbitlen((idx_t)s);

        /* Binary complement: mask = 2^L - 1. */
        seq_t mask = ((seq_t)1 << L) - 1;
        seq_t comp = mask - s;

        /* Check if comp is a perfect cube. */
        seq_t r   = floor_cube_root(comp);
        seq_t r3  = r * r * r;
        if (r3 == comp) {
            /* Store s in the cache, growing if necessary. */
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 64;
                terms = mem_resize(seq_t, terms, terms_cap);
            }
            terms[terms_len++] = s;
        }

        ++next_root;
    }

    return terms[n - 1];
}
