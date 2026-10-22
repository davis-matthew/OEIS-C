/*
 * A313803  Coordination sequence Gal.6.210.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + f(n), for n ≥ 1 where f(n) = 6*floor(n/12) + max(0,
 *            min(n mod 12, 9) - 4 + 1) and a(0) = 1. This implements the
 *            coordination sequence Gal.6.210.2 (OEIS A313803)
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute f(n) */
    idx_t full = n / 12;      /* number of complete periods of 12 */
    idx_t r    = n % 12;      /* remainder in current period */

    /* partial = number of indices in the remainder that lie in [4,9] */
    seq_t partial = 0;
    if (r >= 4) {
        partial = (r <= 9) ? (seq_t)(r - 3) : (seq_t)6;
    }

    /* a(n) = 5*n + 6*full + partial */
    seq_t res = (seq_t)5 * (seq_t)n + (seq_t)6 * (seq_t)full + partial;
    return res;
}
