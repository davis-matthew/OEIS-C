/*
 * A251759  10-step Fibonacci sequence starting with 0,0,0,0,0,0,0,0,1,0.
 * https://oeis.org/A251759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+10) =
 *            a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6)+a(n+7)+a(n+8)+a(n+9),
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=0, a(7)=0,
 *            a(8)=1, a(9)=0
 * Range:     u8 n <= 17, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial 10 terms */
    static const seq_t init[10] = {0,0,0,0,0,0,0,0,1,0};

    /* return early for the first 10 indices */
    if (n < 10) return init[(idx_t)n];

    /* buffer of the last 10 computed values */
    seq_t buf[10];
    for (int i = 0; i < 10; ++i) buf[i] = init[i];

    /* compute from index 10 up to n */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t sum = 0;
        for (int j = 0; j < 10; ++j) sum += buf[j];

        /* rotate buffer left and append new value */
        for (int j = 0; j < 9; ++j) buf[j] = buf[j + 1];
        buf[9] = sum;
    }

    return buf[9];
}
