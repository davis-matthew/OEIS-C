/*
 * A310457  Coordination sequence Gal.6.345.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + c(n) where c(n) = 1 if n == 0 or n % 12 == 11 c(n)
 *            = -1 if n % 12 == 1 c(n) = 0 otherwise (This reproduces a(0)=1,
 *            a(1)=4, a(2)=10, … )
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0u)                 /* a(0) = 1 */
        return 1;
    seq_t val = (seq_t)n * 5;    /* base term 5n */
    if (n % 12 == 1u)
        val -= 1;
    else if (n % 12 == 11u)
        val += 1;
    return val;
}
