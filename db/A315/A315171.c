/*
 * A315171  Coordination sequence Gal.6.220.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0 * a(n) = 4*n + 4*floor(n/10) + 2*(n%10!=0)
 *            for n > 0
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t q = (seq_t)(n / 10);          /* floor(n/10)   */
    seq_t r = (seq_t)(n % 10);          /* n mod 10      */
    return (seq_t)(4 * (seq_t)n) + 4 * q + (r ? 2 : 0);
}
