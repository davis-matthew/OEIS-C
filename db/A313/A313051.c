/*
 * A313051  Coordination sequence Gal.3.33.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0..6) = {1,4,9,14,18,23,28}; For n ≥ 7, the successive
 *            increments repeat the 7‑term pattern P = {4, 4, 5, 5, 4, 5, 5}.
 *            Thus a(n) = a(6) + ⌊(n-7)/7⌋ · 32 + Σ_{i=0}^{(n-7)%7} P[i]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef SEQ_A313051_H
#define SEQ_A313051_H

seq_t a(idx_t n)
{
    static const seq_t init[7] = {1, 4, 9, 14, 18, 23, 28};
    if (n < 7) return init[n];

    /* pattern of increments for n ≥ 7 */
    static const seq_t inc[7]   = {4, 4, 5, 5, 4, 5, 5};
    /* prefix sums of the pattern, used for the remainder of a cycle */
    static const seq_t pref[7]  = {4, 8, 13, 18, 22, 27, 32};

    idx_t k = (n - 7) / 7;          /* number of complete 7‑step cycles */
    idx_t r = (n - 7) % 7;          /* position inside the current cycle */

    /* a(6) = 28, each full cycle adds 32 */
    seq_t result = init[6] + (seq_t)k * (seq_t)32 + pref[r];
    return result;
}
#endif
