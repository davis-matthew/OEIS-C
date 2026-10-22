/*
 * A327100  BII-numbers of antichains of sets with cut-connectivity 1.
 * https://oeis.org/A327100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   dfs_art_helper, is_bii_antichain_cutconn1, seq_alloc,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A327100 – BII-numbers of antichains of sets with cut‑connectivity 1  
   Computed by testing successive integers for the property described in
   the OEIS entry.  For each integer m we construct its binary‑index set system,
   verify that it is an antichain, that it is connected, and that it has an
   articulation point (vertex cut of size 1).  When such an integer is
   found it is appended to the static sequence cache.  The function a(n)
   returns the n‑th element (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/dfs_art_helper.h"
#include "../utils/is_bii_antichain_cutconn1.h"

/* --------------------------------------------------------------------- */
/* Helper: depth‑first search for articulation points                    */

/* --------------------------------------------------------------------- */
/* Test whether an integer m satisfies the required BII‑antichain property */

/* --------------------------------------------------------------------- */
/* Main sequence generator                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of generated terms */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;
    static idx_t seq_cap = 0;
    static uint64_t next = 1;      /* next integer candidate */

    while (seq_len < n) {
        /* grow cache if necessary */
        if (seq_len == seq_cap) {
            idx_t new_cap = seq_cap ? seq_cap * 2 : 4;
            seq = seq_realloc(seq, seq_cap, new_cap);
            seq_cap = new_cap;
        }

        /* find next qualifying integer */
        while (!is_bii_antichain_cutconn1(next))
            ++next;

        /* append to cache */
        seq[seq_len++] = (seq_t)next;
        ++next;
    }
    return seq[n - 1];
}
