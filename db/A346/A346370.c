/*
 * A346370  Upper bound for the number of solutions of the TRINTUM cube puzzle
 *          n X 1 X 1 (cuboid formed by 4n + 2 parts) different by the set of
 *          parts, which are distinguished by the amount of surface area they
 *          contribute to the assembled cuboid.
 * https://oeis.org/A346370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 2)*(floor(n/2) + 1) + floor(floor(n/2)^2/4)
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t m = n / 2;

    /* compute (n + 2) * (m + 1) */
    seq_t part1 = ((seq_t)(n + 2)) * ((seq_t)(m + 1));

    /* compute floor(m^2 / 4) */
    seq_t mm = (seq_t)m;
    seq_t part2 = (mm * mm) / 4;

    return part1 + part2;
}
