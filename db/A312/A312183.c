/*
 * A312183  Coordination sequence Gal.4.42.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=8, a(3)=13, for n≥4
 *            a(n)=a(n-1)+[6,5,4,4,4,4,5] repeating every 7 steps
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;

    /* n ≥ 3 */
    idx_t k = n - 3;                 /* offset from a(3) */
    idx_t cycles = k / 7;            /* number of full 7‑step cycles */
    idx_t rem = k % 7;               /* remaining steps within a cycle */

    /* base value at a(3) plus whole cycles */
    seq_t base = (seq_t)13 + (seq_t)cycles * (seq_t)32;

    /* cumulative sums of increments for the cycle */
    static const seq_t prefix[7] = {0, 6, 11, 15, 19, 23, 27};

    return base + prefix[rem];
}
