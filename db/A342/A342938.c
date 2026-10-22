/*
 * A342938  a(n) is the Skolem number of the cycle graph C_n.
 * https://oeis.org/A342938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)/2) - floor((n+3)/6) + floor((n+2)/6) * +
 *            floor((n+4)/8) - floor(n/8) - floor((n+9)/24) * +
 *            floor((n+8)/24) - floor((n+3)/24) + floor((n+2)/24) (OEIS
 *            A342938)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t xn = (seq_t)n;           /* use seq_t for all arithmetic */
    seq_t res = 0;

    res += (xn + (seq_t)1) / (seq_t)2;
    res -= (xn + (seq_t)3) / (seq_t)6;
    res += (xn + (seq_t)2) / (seq_t)6;
    res += (xn + (seq_t)4) / (seq_t)8;
    res -= xn / (seq_t)8;
    res -= (xn + (seq_t)9) / (seq_t)24;
    res += (xn + (seq_t)8) / (seq_t)24;
    res -= (xn + (seq_t)3) / (seq_t)24;
    res += (xn + (seq_t)2) / (seq_t)24;

    return res;
}
