/*
 * A312680  Coordination sequence Gal.4.63.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 2*count_{i∈[3,n]}(i mod 7 ∈ {3,4,5}) (n ≥ 0) Base
 *            case: a(0) = 1 For n ≥ 1, the closed‑form simplifies to: a(n) =
 *            4*n + 2*cnt6(n) where cnt6(n) = floor(n/7)*3 + [n%7 ≥ 3] + [n%7
 *            ≥ 4] + [n%7 ≥ 5]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;               /* a(0) = 1 */
    idx_t q = n / 7;                    /* complete blocks of 7 */
    idx_t r = n % 7;                    /* remainder in the last block */
    /* number of indices i in [3,n] with i%7 == 3,4,5 */
    idx_t cnt6 = q * 3 + (r >= 3) + (r >= 4) + (r >= 5);
    /* compute a(n) in seq_t to avoid overflow in intermediate multiplications */
    return (seq_t)n * 4 + (seq_t)cnt6 * 2;
}
