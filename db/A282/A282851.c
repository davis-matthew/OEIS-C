/*
 * A282851  35-gonal numbers: a(n) = n*(33*n-31)/2.
 * https://oeis.org/A282851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(33*n-31)/2
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * ((((seq_t)33 * nn) - (seq_t)31));
    val /= (seq_t)2;
    return val;
}
