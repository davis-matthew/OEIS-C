/*
 * A313786  Coordination sequence Gal.4.128.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 5 a(2) = 10 For n ≥ 2: if n is even: a(n) = 5*n
 *            + n/2 - 1 if n is odd and n <= 3: a(n) = 5*n if n is odd and n ≥
 *            5: a(n) = 5*n + 1
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if ((n & 1) == 0) {
        // even n
        seq_t term = (seq_t)n * (seq_t)5;
        term += (seq_t)(n >> 1);   // n/2
        term -= (seq_t)1;
        return term;
    } else {
        // odd n
        if (n <= 3) {
            return (seq_t)n * (seq_t)5;
        } else {
            seq_t term = (seq_t)n * (seq_t)5;
            term += (seq_t)1;
            return term;
        }
    }
}
