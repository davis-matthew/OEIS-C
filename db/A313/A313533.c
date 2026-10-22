/*
 * A313533  Coordination sequence Gal.6.131.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 54 * floor(n/12) + base[n mod 12], where base[0] = 1,
 *            base[1] = 5, base[2] = 10, base[3] = 14, base[4] = 19, base[5] =
 *            23, base[6] = 27, base[7] = 31, base[8] = 35, base[9] = 40,
 *            base[10] = 44, base[11] = 49. For n = 0 this yields a(0) = 1.
 *            For n > 0 with n % 12 == 0, the term is 54 * (n/1...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[12] = {
        1, 5, 10, 14, 19, 23, 27, 31, 35, 40, 44, 49
    };

    if (n < 12)
        return base[n];

    idx_t k = n / 12;          /* number of 12‑step periods */
    seq_t res = 54 * (seq_t)k; /* each period adds 54 */

    idx_t r = n % 12;
    if (r != 0)
        res += base[r];

    return res;
}
