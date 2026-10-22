/*
 * A313681  Coordination sequence Gal.6.221.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 1 + floor((max(0, n-2)/11))*6 + incPrefix[(n-2)%11]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where incPrefix = {1,2,2,2,2,2,2,3,4,5,6} (number of 5‑increments in the first r+1 positions of the 11‑period pattern). */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)4 * (seq_t)n + 1;

    if (n >= 2) {
        idx_t k = (n - 2) / 11;
        idx_t r = (n - 2) % 11;
        static const unsigned char incPrefix[11] = {1, 2, 2, 2, 2, 2, 2, 3, 4, 5, 6};
        val += (seq_t)(k * 6 + incPrefix[r]);
    }

    return val;
}
