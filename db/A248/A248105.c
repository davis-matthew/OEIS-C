/*
 * A248105  Positions of 1,0,1 in the Thue-Morse sequence (A010060).
 * https://oeis.org/A248105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   parity_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes positions of the pattern 1,0,1 in the Thue‑Morse sequence
   (OEIS A248105) by scanning the sequence as needed.  The function
   remembers previously found positions so that calls with arbitrary n
   are O(1) after the required entries have been discovered. */
#include <stdlib.h>
#include "../utils/parity_idx.h"

seq_t a(idx_t n)
{
    /* Static cache of found positions */
    static seq_t *pos_cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t scan_pos = 1;   /* next position to examine */

    if (n == 0) return 0;   /* undefined for n<1; return 0 for safety */

    /* Grow cache until we have at least n entries */
    while (cache_size < n) {
        /* Compute bits at scan_pos, scan_pos+1, scan_pos+2 */
        seq_t b0 = parity_idx(scan_pos);
        seq_t b1 = parity_idx(scan_pos + 1);
        seq_t b2 = parity_idx(scan_pos + 2);

        if (b0 == 1 && b1 == 0 && b2 == 1) {
            /* Append to cache */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(pos_cache, new_cap * sizeof(seq_t));
                pos_cache = tmp;
                cache_cap = new_cap;
            }
            pos_cache[cache_size++] = scan_pos;
        }
        /* Advance to next starting position */
        scan_pos++;
    }

    return pos_cache[n - 1];
}
