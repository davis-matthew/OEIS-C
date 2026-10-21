/*
 * A264894  a(n) = n*(7*n - 5)*(49*n^2 - 35*n - 10)/8.
 * https://oeis.org/A264894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n - 5)*(49*n^2 - 35*n - 10)/8
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 99, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t p1 = (seq_t)7 * nn - (seq_t)5;
    seq_t p2 = (seq_t)49 * nn * nn - (seq_t)35 * nn - (seq_t)10;
    seq_t result = nn * p1;
    result = result * p2;
    result = result / (seq_t)8;
    return result;
}
