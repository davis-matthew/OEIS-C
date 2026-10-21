/*
 * A275874  a(n) = (n-4)*(n+1)*(n+3)/6.
 * https://oeis.org/A275874
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-4)*(n+1)*(n+3)/6
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;
    seq_t N = (seq_t)n;
    seq_t t1 = N - 4;
    seq_t t2 = N + 1;
    seq_t t3 = N + 3;
    seq_t prod = t1 * t2;
    prod *= t3;
    return prod / 6;
}
