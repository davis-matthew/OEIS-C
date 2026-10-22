/*
 * A315246  Coordination sequence Gal.6.203.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n > 0: a(n) = 5*n + f(n mod 12) where f = {0, 1,
 *            0, 0, -1, 0, 0, 0, 1, 0, 0, -1} (indices 0..11)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* periodic adjustment pattern for 5*n */
    static const int f[12] = {0, 1, 0, 0, -1, 0, 0, 0, 1, 0, 0, -1};

    int r = (int)(n % 12);
    /* use signed intermediate to allow subtraction of 1 */
    long long temp = (long long)5 * (long long)n + f[r];
    return (seq_t)temp;
}
