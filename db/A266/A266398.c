/*
 * A266398  Number of orbits of Aut(Z^7) as function of the infinity norm n of
 *          the representative lattice point of the orbit, when the
 *          cardinality of the orbit is equal to 13440.
 * https://oeis.org/A266398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 30*n^2 - 97*n + 66)/6
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = n3 + (seq_t)30 * n2 - (seq_t)97 * nn + (seq_t)66;
    return num / (seq_t)6;
}
