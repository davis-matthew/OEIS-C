/*
 * A323064  Squares whose binary complement (A035327) is a cube.
 * https://oeis.org/A323064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   bitlen_seq_t, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by exhaustive search: squares whose binary complement is a perfect cube. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/floor_cube_root.h"

/* Cache for the sequence. */
static seq_t *terms      = NULL;   /* array of found squares            */
static size_t terms_len  = 0;       /* number of elements in terms       */
static size_t terms_cap  = 0;       /* allocated capacity for terms      */
static idx_t  next_root  = 0;       /* next integer to test as a root    */

static inline unsigned int intbitlen(idx_t x) {
    return (x == 0) ? 1 : (unsigned int)bitlen_seq_t(x);
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
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = s;
        }

        ++next_root;
    }

    return terms[n - 1];
}
