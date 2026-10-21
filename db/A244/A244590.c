/*
 * A244590  a(n) = sum( floor(k*n/8), k=1..7 ).
 * https://oeis.org/A244590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum( floor(k*n/8), k=1..7 )
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sum = 0;
    sum += (nn * 1) / 8;
    sum += (nn * 2) / 8;
    sum += (nn * 3) / 8;
    sum += (nn * 4) / 8;
    sum += (nn * 5) / 8;
    sum += (nn * 6) / 8;
    sum += (nn * 7) / 8;
    return sum;
}
