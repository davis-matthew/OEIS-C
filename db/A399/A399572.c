/*
 * A399572  Triangular numbers in the Padovan sequence A000931.
 * https://oeis.org/A399572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, new
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A399572: Padovan numbers that are triangular (i.e., 8*P(k)+1 is a perfect square). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Cache of found triangular Padovan numbers */
    static seq_t *tri = NULL;
    static size_t tri_cap = 0;
    static size_t tri_len = 0;

    /* Padovan recurrence state */
    static seq_t pad[3];
    static idx_t pad_idx = 0;
    static bool init = false;

    /* Initialise Padovan sequence (P(0)=0, P(1)=1, P(2)=1) */
    if (!init) {
        pad[0] = (seq_t)0;
        pad[1] = (seq_t)1;
        pad[2] = (seq_t)1;
        pad_idx = 0;
        init = true;
    }

    /* Generate Padovan numbers until we have at least n found terms,
       or we break after a safety limit because the sequence is finite. */
    while (tri_len < (size_t)n) {
        seq_t p_val;
        if (pad_idx < 3) {
            p_val = pad[pad_idx];
        } else {
            seq_t v1 = pad[(pad_idx - 2) % 3];
            seq_t v2 = pad[(pad_idx - 3) % 3];
            p_val = v1 + v2;
            pad[pad_idx % 3] = p_val;
        }

        /* Test whether p_val is triangular:
           8*p_val + 1 should be a perfect square. */
        seq_t s = p_val * (seq_t)8 + (seq_t)1;
        seq_t r = floor_sqrt(s);
        if (r * r == s) {
            /* Store the value if it wasn't already stored immediately before. */
            if (tri_len == 0 || p_val != tri[tri_len - 1]) {
                if (tri_len == tri_cap) {
                    tri_cap = tri_cap == 0 ? 4 : tri_cap * 2;
                    tri = (seq_t *)realloc(tri, tri_cap * sizeof(seq_t));
                }
                tri[tri_len++] = p_val;
            }
        }

        /* Advance to next Padovan index */
        pad_idx++;

        /* Safety break: the sequence is known to be finite (8 terms). */
        if (pad_idx > 20000) {
            break;
        }
    }

    if (n == 0 || (size_t)n > tri_len) {
        return (seq_t)0;
    }
    return tri[n - 1];
}
