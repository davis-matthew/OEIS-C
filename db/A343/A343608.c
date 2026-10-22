/*
 * A343608  a(n) = [n/5]*[n/5 - 1]*(3n - 10[n/5 + 1])/6, where [.] = floor:
 *          upper bound for minimum number of monochromatic triangles in a
 *          3-edge-colored complete graph K_n.
 * https://oeis.org/A343608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/5) * (floor(n/5) - 1) * (3*n - 10*(floor(n/5)+1))
 *            / 6
 * Range:     u8 n <= 23, u16 n <= 218, u32 n <= 4974, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n / 5;          /* floor(n/5) */
    seq_t t1 = tn + 1;                /* floor(n/5)+1  */
    seq_t term = 3 * (seq_t)n - 10 * t1;
    seq_t res = tn * (tn - 1);
    res = res * term;
    res = res / 6;                    /* exact division */
    return res;
}
