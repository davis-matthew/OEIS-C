/*
 * A310238  Coordination sequence Gal.4.7.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n >= 1: a(n) = 4*n + g(n) where g(n) = -1 if n mod
 *            10 is 1 or 2, +1 if n mod 10 is 8 or 9, 0 otherwise
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * 4;          /* 4*n in seq_t */
    if (n == 0) {
        val += 1;                      /* a(0) = 1 */
    } else {
        idx_t r = n % 10;              /* last digit of n in base 10 */
        if (r == 1 || r == 2) {
            val -= 1;
        } else if (r == 8 || r == 9) {
            val += 1;
        }
        /* all other residues contribute 0 */
    }
    return val;
}
