/*
 * A363080  Number of hexagonal lattice points within a hexagram centered at a
 *          lattice point and with outermost vertices at the six lattice
 *          points n steps outward from the central point.
 * https://oeis.org/A363080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*ceil(n*(n+1)/3) + 1
 * Range:     u8 n <= 10, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n*(n+1) */
    seq_t t = (seq_t)n * ((seq_t)n + 1);

    /* Ceil division by 3: ceil(t/3) == (t + 2) / 3 for non-negative t */
    seq_t c = (t + 2) / 3;

    /* a(n) = 6*c + 1 */
    return 6 * c + 1;
}
