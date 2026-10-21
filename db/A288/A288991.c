/*
 * A288991  Fixed point of the mapping 00->0010, 01->001, 10->000, starting
 *          with 00.
 * https://oeis.org/A288991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: a(n) is the nᵗʰ bit of the infinite word obtained by
   repeatedly applying the morphism 00 → 0010, 01 → 001, 10 → 000
   to the seed “00”.  The word is built iteratively until it contains
   at least n bits, then the nᵗʰ bit is returned. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined offset */
    size_t target = (size_t)n;

    /* start with the seed "00" */
    size_t cur_len = 2;
    uint8_t *cur = (uint8_t *)calloc(cur_len, sizeof(uint8_t));
    if (!cur) return 0;                   /* allocation failure */
    cur[0] = 0;
    cur[1] = 0;

    while (cur_len < target) {
        size_t pairs   = cur_len / 2;     /* number of non‑overlapping pairs */
        size_t max_out = pairs * 4;      /* worst case: each pair expands to 4 bits */

        uint8_t *next = (uint8_t *)malloc(max_out * sizeof(uint8_t));
        if (!next) { free(cur); return 0; }

        size_t pos = 0;
        for (size_t i = 0; i + 1 < cur_len; i += 2) {
            if (cur[i] == 0 && cur[i+1] == 0) {   /* 00 → 0010 */
                next[pos++] = 0;
                next[pos++] = 0;
                next[pos++] = 1;
                next[pos++] = 0;
            } else if (cur[i] == 0 && cur[i+1] == 1) { /* 01 → 001 */
                next[pos++] = 0;
                next[pos++] = 0;
                next[pos++] = 1;
            } else if (cur[i] == 1 && cur[i+1] == 0) { /* 10 → 000 */
                next[pos++] = 0;
                next[pos++] = 0;
                next[pos++] = 0;
            }
            /* pair 11 never appears in this construction, so it is ignored */
        }

        free(cur);
        cur = next;
        cur_len = pos;
    }

    uint8_t val = cur[n-1];
    free(cur);
    return (seq_t)val;
}
