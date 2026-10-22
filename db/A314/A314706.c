/*
 * A314706  Coordination sequence Gal.6.130.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 + 4*n + |{k ≤ n | k∈S}|
 *   where S = {4,9,12,13,16,21,24,25,28,33,36,37,40,45,48,49,52,57,60,61,64,69,…}
 *   S consists of blocks of 4 indices
 *   block t (t ≥ 0): 4 + 12*t , 9 + 12*t , 12 + 12*t , 13 + 12*t
 *   thus a(n) can be computed in O(1) time.
 */

#include <stddef.h>

static const idx_t K_OFFSETS[4] = {0,5,8,9};

seq_t a(idx_t n) {
    /* base case: the first term a(0) = 1 */
    if (n <= 0) return (seq_t)1;

    /* count how many indices ≤ n belong to S */
    idx_t cntS = 0;
    if (n >= 4) {
        /* number of complete 12‑step blocks before n */
        idx_t block = (n - 4) / 12;
        cntS = 4 * block;            /* 4 indices per complete block */

        /* count indices in the partial block that contains n */
        idx_t base = 4 + 12 * block;  /* first index of the block */
        for (int i = 0; i < 4; ++i) {
            idx_t val = base + K_OFFSETS[i];
            if (val <= n)
                ++cntS;
            else
                break;            /* offsets are increasing */
        }
    }

    /* a(n) = 1 + 4*n + cntS */
    return (seq_t)1 + (seq_t)4 * ((seq_t)n) + (seq_t)cntS;
}
