/*
 * A271566  a(n) is the length of the n-th run in A137734.
 * https://oeis.org/A271566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=3; for n>2: if n%3==0: a(n)=1; if n%3==1:
 *            a(n)=(n-1)/3; if n%3==2: a(n)=(n-2)/3+2
 * Range:     u8 n <= 763, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    switch ((int)(n % 3))
    {
        case 0:
            return (seq_t)1;
        case 1:
            return (seq_t)((n - 1) / 3);
        case 2:
            return (seq_t)(((n - 2) / 3) + 2);
    }
    /* unreachable */
    return (seq_t)0;
}
