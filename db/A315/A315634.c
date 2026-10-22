/*
 * A315634  Coordination sequence Gal.4.55.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n=0) * 6 (n=1) * 12 (n=2) * 12 + ⌊(n-2)/7⌋·36 +
 *            min(r,3)·4 + max(r-3,0)·6 (n≥3) * where r = (n-2) mod 7. * This
 *            reproduces the sequence: *
 *            1,6,12,16,20,24,30,36,42,48,52,56,60,66,72,78,84,88,92,96,102,108,114,120,
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)12;

    idx_t m = n - 2;          /* number of diffs starting at k=3 up to k=n */
    idx_t t = m / 7;          /* full 7‑term cycles of the pattern */
    idx_t r = m % 7;          /* remainder of the pattern */

    seq_t sum = (seq_t)t * 36;   /* each full cycle contributes 36 */

    if (r < 3) {                     /* only 4‑step diffs in the remainder */
        sum += (seq_t)r * 4;
    } else {                         /* 3×4‑step diffs plus (r‑3)×6‑step diffs */
        sum += (seq_t)3 * 4;         /* 12 from the 3×4 step part */
        sum += (seq_t)(r - 3) * 6;
    }

    return (seq_t)12 + sum;
}
