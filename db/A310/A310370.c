/*
 * A310370  Coordination sequence Gal.5.109.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(0) = 1 a(1) = 4 For n ≥ 2,
 *            a(n) = a(n‑1) + d(n) where the difference sequence d(n) begins
 *            as 6,4,4,4,6,4,4,6,4,4,4,4,6,4,4,6,… (i.e. a 6 followed by 4
 *            4’s, then a 6 followed by 2 4’s, alternating the block lengths 4
 *            and 2)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t val = (seq_t)4;          // a(1)
    bool nextIsSix = true;         // next difference is 6 (for a(2))
    int  blockParity = 0;          // 0 → block of 4 4’s, 1 → block of 2 4’s
    int  remaining4s = 0;          // 4’s to emit in current block

    for (idx_t i = 2; i <= n; ++i) {
        seq_t diff = nextIsSix ? (seq_t)6 : (seq_t)4;
        val += diff;

        if (nextIsSix) {
            nextIsSix = false;
            remaining4s = (blockParity == 0) ? 4 : 2;
        } else {
            if (--remaining4s == 0) {
                nextIsSix = true;
                blockParity ^= 1;   // toggle between 4–block and 2–block
            }
        }
    }
    return val;
}
