/*
 * A315427  Coordination sequence Gal.5.133.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = a(n-1) + c(n) for n >= 1 where c(1) = 5 c(n) = 6
 *            if n % 9 == 0 6 if n % 9 == 1 5 if n % 9 == 2 4 if n % 9 == 3 6
 *            if n % 9 == 4 6 if n % 9 == 5 6 if n % 9 == 6 4 if n % 9 == 7 5
 *            if n % 9 == 8
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Increment values for residues 0..8 of i % 9 */
    static const seq_t inc[9] = {
        6, /* i % 9 == 0 */
        6, /* i % 9 == 1 */
        5, /* i % 9 == 2 */
        4, /* i % 9 == 3 */
        6, /* i % 9 == 4 */
        6, /* i % 9 == 5 */
        6, /* i % 9 == 6 */
        4, /* i % 9 == 7 */
        5  /* i % 9 == 8 */
    };

    seq_t result = 1 + 5;          /* a(1) = a(0) + 5 */

    for (idx_t i = 2; i <= n; ++i)
        result += inc[i % 9];

    return result;
}
