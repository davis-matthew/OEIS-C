/*
 * A268906  Number of 3 X n 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 29*a(n-2) + 20*a(n-3) - 4*a(n-4) for n>6,
 *            a(1)=0, a(2)=240, a(3)=1584, a(4)=9720, a(5)=54936, a(6)=299088
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices below the offset */
    if (n < 1) return 0;

    /* Initial terms */
    switch (n) {
        case 1: return (seq_t)0;
        case 2: return (seq_t)240;
        case 3: return (seq_t)1584;
        case 4: return (seq_t)9720;
        case 5: return (seq_t)54936;
        case 6: return (seq_t)299088;
    }

    /* Use a sliding window of the last four values */
    seq_t a1 = (seq_t)1584;   /* a(3) */
    seq_t a2 = (seq_t)9720;   /* a(4) */
    seq_t a3 = (seq_t)54936;  /* a(5) */
    seq_t a4 = (seq_t)299088; /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 10 * a4 - 29 * a3 + 20 * a2 - 4 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
