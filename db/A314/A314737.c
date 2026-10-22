/*
 * A314737  Coordination sequence Gal.6.260.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314737
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

/* OEIS A314737 – Coordination sequence Gal.6.260.3
 * Formula: a(0..11) = {1,5,9,13,18,24,29,34,40,45,49,53}
 *            a(12)   = 58
 *            a(n) = a(n-12) + 58   for all n ≥ 13
 * The recurrence is equivalent to:
 *   for n ≥ 12
 *       let m = n - 12
 *       a(n) = 58 * (⌊m/12⌋ + 1) + base[m mod 12]
 *   with base[0] = 0 and base[1..11] = {5,9,13,18,24,29,34,40,45,49,53}
 */

#include <stddef.h>  /* for size_t */

static const seq_t _small_base[12] = {
    1, 5, 9, 13, 18, 24, 29, 34, 40, 45, 49, 53
};

static const seq_t _base_mod[12] = {
    0, 5, 9, 13, 18, 24, 29, 34, 40, 45, 49, 53
};

seq_t a(idx_t n)
{
    if (n <= 11u)
        return _small_base[(size_t)n];

    idx_t delta    = n - 12u;
    idx_t cycles   = delta / 12u;            /* number of full 12‑step blocks after n=12 */
    seq_t add      = (seq_t)cycles + 1u;      /* 1 for the first block (n=12–23) */
    int    offset  = (int)(delta % 12u);

    return 58u * add + _base_mod[(size_t)offset];
}
