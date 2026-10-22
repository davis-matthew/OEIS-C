/*
 * A383580  a(n) is the largest number t such that there exist numbers i,j,k,l
 *          such that, for all m <= t, there exist integers x,y,z,w with x*i +
 *          y*j + z*k + w*l = m and |x|+|y|+|z|+|w| <= n.
 * https://oeis.org/A383580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = max_{m=0..n-1} ( 2*n^2*m^2 - 4*n*m^3 + 2*m^4 + 4*n^2*m -
 *            6*n*m^2 + 2*m^3 + 2*n^2 - 2*n*m + m^2 + n ) for n > 3. Initial
 *            values: a(0)=0, a(1)=4, a(2)=16, a(3)=47
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 4;
    if (n == 2) return 16;
    if (n == 3) return 47;

    seq_t best = 0;
    idx_t m_end = n - 1;
    for (idx_t m = 0; m <= m_end; ++m) {
        seq_t nn = (seq_t)n;
        seq_t mm = (seq_t)m;
        seq_t val =
            2 * nn * nn * mm * mm
            - 4 * nn * mm * mm * mm
            + 2 * mm * mm * mm * mm
            + 4 * nn * nn * mm
            - 6 * nn * mm * mm
            + 2 * mm * mm * mm
            + 2 * nn * nn
            - 2 * nn * mm
            + mm * mm
            + nn;
        if (val > best) best = val;
    }
    return best;
}
