/*
 * A259777  Number of permutations p of [n] with no fixed points and
 *          displacement of elements restricted by four: 1 <= |p(i)-i| <= 4.
 * https://oeis.org/A259777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A259777 – number of permutations of [n] with no fixed points
 *  and |p(i)-i| in {1,2,3,4}.  Computed by a sliding‑window DP.
 *
 *  DP state: 9‑bit mask (bits 0..8) representing the usage of
 *  images j in the window [i-4, i+4] before assigning position i.
 *  After assigning, the mask is shifted right by 1 and a new right‑most
 *  bit is set to 1 if the new image index exceeds n.
 *
 *  The algorithm runs in O(n·512) time and uses O(512) memory.
 */

#include <stdint.h>
#include <stdbool.h>

#define MASK_SIZE (1U << 9)   /* 512 masks – 9 bits */

seq_t a(idx_t n) {
    /* Offset 0: only the empty permutation is allowed. */
    if (n == 0U) return (seq_t)1;

    /* Arrays for DP: we only need two consecutive layers. */
    seq_t dp_cur[MASK_SIZE] = {0};
    seq_t dp_next[MASK_SIZE] = {0};

    /* --- initialise mask for position i = 1 ---------------------- */
    uint32_t initMask = 0;
    /* Images j < 1 are considered already used. */
    initMask |= 0xFU;                  /* bits 0..3 set */

    /* Image j = 1 is free, so bit 4 is 0. */

    /* Images j = 2..5 (bits 5..8) – mark as used if they exceed n. */
    for (uint32_t k = 5; k <= 8; ++k) {
        idx_t j = (idx_t)1 - 4 + k;    /* j = 1-4 + k = k-3 */
        if (j > n) initMask |= (1U << k);
    }
    dp_cur[initMask] = (seq_t)1;

    /* --- main DP loop ------------------------------------------- */
    for (idx_t i = 1U; i <= n; ++i) {
        /* zero next layer */
        for (uint32_t m = 0; m < MASK_SIZE; ++m) dp_next[m] = (seq_t)0;

        for (uint32_t m = 0; m < MASK_SIZE; ++m) {
            seq_t ways = dp_cur[m];
            if (!ways) continue;

            /* try to assign position i to all possible images j */
            for (uint32_t k = 0; k < 9; ++k) {
                if (k == 4) continue;                /* cannot map to itself */
                if (m & (1U << k)) continue;         /* image already used */

                /* compute the absolute image index j */
                idx_t j = i - 4 + k;
                if (j < 1U || j > n) continue;       /* out of bounds */

                uint32_t m2 = m | (1U << k);         /* mark image j as used */
                uint32_t next = m2 >> 1;              /* shift window right */

                /* new image entering the window: index i+5 */
                idx_t newIdx = i + 5U;
                if (newIdx > n) next |= (1U << 8);

                dp_next[next] += ways;
            }
        }

        /* swap current and next */
        for (uint32_t m = 0; m < MASK_SIZE; ++m) dp_cur[m] = dp_next[m];
    }

    /* After processing all positions, all images 1..n must be used.
     * The final window [n-4, n+4] contains all those indices, so
     * the mask must be all 1s (511).  Any out‑of‑range indices are
     * already set to 1, hence we simply look at mask 511. */
    return dp_cur[511];
}
