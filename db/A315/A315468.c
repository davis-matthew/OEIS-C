/*
 * A315468  Coordination sequence Gal.5.300.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + 1 + floor((n-10)/10 + 1 if n>9) + floor((n-1)/10 if
 *            n>10)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (i.e. 5*n + 1 plus the number of indices i< n that are 9 mod 10 or 0 mod 10, with i≥10 for the 0 mod 10 case) */

seq_t a(idx_t n)
{
    /* base value 5*n + 1 */
    seq_t result = (seq_t)5 * (seq_t)n + 1;

    /* add the extra 1's produced when a step of size 6 occurs */
    if (n > 9) {
        /* indices 9, 19, 29, ... up to n-1 */
        result += (seq_t)((n - 10) / 10 + 1);
    }
    if (n > 10) {
        /* indices 10, 20, 30, ... up to n-1 */
        result += (seq_t)((n - 1) / 10);
    }

    return result;
}
