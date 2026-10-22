/*
 * A334274  Numbers k such that the k-th composition in standard order is both
 *          a necklace and a reversed co-necklace.
 * https://oeis.org/A334274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   compose_from_k, is_rev_co_necklace
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth element of OEIS sequence A334274.
   a(n) is the n-th non‑negative integer k such that
   the k‑th composition in standard order is both a necklace
   and a reversed co‑necklace.

   The algorithm enumerates non‑negative integers k,
   transforms each k to its standard composition,
   checks the two combinatorial properties, and
   stores those k that satisfy both until the requested
   index is reached.  Results are memoised so that
   subsequent calls, in any order, are fast.  The routine
   uses only the arithmetic and bitwise operators permitted
   in the problem statement. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/compose_from_k.h"
#include "../utils/is_rev_co_necklace.h"

/* Test whether a composition is a necklace:
   it must be lexicographically <= any cyclic rotation. */
static bool is_necklace(const size_t *p, size_t len)
{
    if (len == 0) return true;
    for (size_t s = 1; s < len; ++s) {
        for (size_t i = 0; i < len; ++i) {
            size_t a = p[i];
            size_t b = p[(i + s) % len];
            if (a < b) break;          /* p < rotation => OK for this s */
            if (a > b) return false;   /* p > rotation => fail */
        }
    }
    return true;
}

/* Memoised search for the n‑th element of the sequence. */
static seq_t *matches = NULL;      /* stored k values */
static size_t matches_len = 0;     /* number of stored values */
static size_t matches_cap = 0;     /* allocated capacity */
static idx_t next_k = 0;           /* next k to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused; offset is 1 */

    /* Grow cache to hold up to index n. */
    while (matches_len < (size_t)n) {
        size_t comp[64];
        size_t len;
        compose_from_k(next_k, comp, &len);

        if (is_necklace(comp, len) && is_rev_co_necklace(comp, len)) {
            if (matches_len == matches_cap) {
                size_t new_cap = matches_cap == 0 ? 64 : matches_cap * 2;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (tmp == NULL) exit(1);          /* allocation failed */
                if (matches) memcpy(tmp, matches, matches_len * sizeof(seq_t));
                free(matches);
                matches = tmp;
                matches_cap = new_cap;
            }
            matches[matches_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return matches[n - 1];
}
