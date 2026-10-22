/*
 * A302803  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 24*a(n-3) - 144*a(n-4) (n>6) Initial values:
 *            a(1)=4, a(2)=29, a(3)=169, a(4)=1010, a(5)=6084, a(6)=36456
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, ignore 0 */
    switch (n)
    {
        case 1: return (seq_t)4;
        case 2: return (seq_t)29;
        case 3: return (seq_t)169;
        case 4: return (seq_t)1010;
        case 5: return (seq_t)6084;
        case 6: return (seq_t)36456;
    }

    /* Use a circular buffer of the last five terms: a1..a6 correspond to
       a(n-5), a(n-4), a(n-3), a(n-2), a(n-1), a(n) respectively. */
    seq_t a1 = (seq_t)4;        /* a(1)  */
    seq_t a2 = (seq_t)29;       /* a(2)  */
    seq_t a3 = (seq_t)169;      /* a(3)  */
    seq_t a4 = (seq_t)1010;     /* a(4)  */
    seq_t a5 = (seq_t)6084;     /* a(5)  */
    seq_t a6 = (seq_t)36456;    /* a(6)  */

    for (idx_t i = 7; i <= n; ++i)
    {
        /* a(i) = 6*a(i-1) + 24*a(i-3) - 144*a(i-4) */
        seq_t next = (seq_t)6 * a6 + (seq_t)24 * a4 - (seq_t)144 * a3;

        /* shift the window: a1<-a2, a2<-a3, a3<-a4, a4<-a5, a5<-a6, a6<-next */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
