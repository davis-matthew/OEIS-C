/*
 * A275365  a(1)=2, a(2)=2; thereafter a(n) = a(n-a(n-1)) + a(n-a(n-2)).
 * https://oeis.org/A275365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0; a(2n)=2; a(2n+1)=2n+2
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if ((n & 1) == 0) return (seq_t)2;
    return (seq_t)n + 1;
}
