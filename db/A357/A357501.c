/*
 * A357501  Length of longest induced cycle in the n X n king graph.
 * https://oeis.org/A357501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=0, a(2)=3, a(3)=4, a(4)=8, a(5)=12, a(6)=16, a(7)=24 For n
 *            >= 8: if n % 4 == 0: a(n) = n^2/2 - 1 if n % 4 == 1: a(n) = (n^2
 *            - 5)/2 if n % 4 == 2: a(n) = n^2/2 - 3 if n % 4 == 3: a(n) =
 *            (n^2 - 1)/2
 * Range:     u8 n <= 15, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 8)
    {
        static const seq_t small[8] = {0, 0, 3, 4, 8, 12, 16, 24};
        return small[n];
    }
    seq_t n1 = (seq_t)n;
    seq_t nsq = n1 * n1;
    switch (n & 3)
    {
        case 0: return nsq / 2 - 1;
        case 1: return (nsq - 5) / 2;
        case 2: return nsq / 2 - 3;
        case 3: return (nsq - 1) / 2;
    }
    return 0; /* unreachable */
}
