/*
 * A313846  Coordination sequence Gal.5.308.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 5 a(2) = 10 For n ≥ 3: cycles = (n - 3) / 9 rem
 *            = (n - 3) % 9 prefix = {0, 4, 11, 15, 21, 26, 31, 36, 41, 47}
 *            a(n) = 16 + cycles * 47 + prefix[rem]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 10;

    /* prefix sums of the 9‑step pattern 4,7,4,6,5,5,5,5,6 */
    static const seq_t prefix[10] = {0, 4, 11, 15, 21, 26, 31, 36, 41, 47};

    idx_t m = n - 3;
    idx_t cycles = m / 9;
    idx_t rem = m % 9;
    return 16 + (seq_t)cycles * 47 + prefix[rem];
}
