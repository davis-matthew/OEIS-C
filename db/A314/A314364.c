/*
 * A314364  Coordination sequence Gal.5.91.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0: if n % 3 == 0 → a(n) = 15 * (n / 3) else:
 *            let k = n / 3 a(n) = (n % 3 == 1 ? 5 : 7) + 25 * (k / 2) + ((k %
 *            2) ? 13 : 0)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)1;
    }

    if ((n % 3u) == 0u) {
        return (seq_t)15 * (seq_t)(n / 3u);
    }

    seq_t k = (seq_t)(n / 3u);            /* integer division */
    seq_t base = ((n % 3u) == 1u) ? (seq_t)5 : (seq_t)7;
    seq_t q = k / 2u;                     /* number of full (13+12) cycles */
    seq_t rem = k % 2u;                   /* whether an extra 13 is needed */
    seq_t inc = 25u * q + (rem ? (seq_t)13 : (seq_t)0);
    return base + inc;
}
