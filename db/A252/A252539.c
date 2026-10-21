/*
 * A252539  Number of (7+2) X (n+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=823, a(2)=1344, a(3)=2332, a(4)=3160, a(5)=10720,
 *            a(n)=8*a(n-3) for n>5
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5) {
        switch ((idx_t)n) {
            case 1: return (seq_t)823;
            case 2: return (seq_t)1344;
            case 3: return (seq_t)2332;
            case 4: return (seq_t)3160;
            case 5: return (seq_t)10720;
        }
    }
    /* recurrence for n>5: a(n)=8*a(n-3) */
    seq_t b1 = (seq_t)2332; /* a(i-3) for the first iteration (i=6) */
    seq_t b2 = (seq_t)3160; /* a(i-2) */
    seq_t b3 = (seq_t)10720;/* a(i-1) */
    seq_t next;
    for (idx_t i = 6; i <= n; ++i) {
        next = b1 * (seq_t)8;   /* a(i-3)*8 = a(i) */
        b1 = b2;                /* shift buffer */
        b2 = b3;
        b3 = next;
    }
    return b3; /* holds a(n) after loop */
}
