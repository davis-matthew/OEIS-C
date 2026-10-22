/*
 * A313781  Coordination sequence Gal.6.617.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 *          a(n) = 5*n + 2*floor(n/12) + (n%12 >= 4) + (n%12 >= 9)   for n >= 1
 *
 * This implements the OEIS sequence A313781 (coordination sequence Gal.6.617.3).
 */
seq_t a(idx_t n) {
    if (n == 0)
        return 1;
    /* 5 * n */
    seq_t res = (seq_t)5 * (seq_t)n;
    /* + 2 * floor(n / 12) */
    res += (seq_t)(n / 12) * (seq_t)2;
    /* + 1 if n % 12 >= 4 */
    idx_t r = n % 12;
    if (r >= 4)
        res += (seq_t)1;
    /* + 1 if n % 12 >= 9 */
    if (r >= 9)
        res += (seq_t)1;
    return res;
}
