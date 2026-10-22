/*
 * A310452  Coordination sequence Gal.6.195.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n - ⌊n/6⌋ - δ(n) where δ(n) = 1 if (n%6==1 or n%6==5)
 *            and ⌊n/6⌋ even, or if (n%6==2,3,4) and ⌊n/6⌋ odd; δ(n) = 0
 *            otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t q = n / 6;      /* floor(n/6) */
    idx_t r = n % 6;      /* n modulo 6 */

    seq_t val = (seq_t)n * (seq_t)5 - (seq_t)q;  /* 5*n - floor(n/6) */

    if (r != 0) {
        if ((r == 1 || r == 5) && ((q & 1) == 0))
            val -= 1;
        else if ((r >= 2 && r <= 4) && ((q & 1) == 1))
            val -= 1;
    }

    return val;
}
