/*
 * A264054  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change +-(.,.) 0,0 1,2 or 2,-2.
 * https://oeis.org/A264054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 3*a(n-3) - 9*a(n-4) + 3*a(n-5) -
 *            a(n-6) + 3*a(n-7) - a(n-8) initial values: a(1) = 2, a(2) = 8,
 *            a(3) = 18, a(4) = 45, a(5) = 125, a(6) = 320, a(7) = 832, a(8) =
 *            2197
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the OEIS sequence A264054.  The implementation uses a
 *  linear recurrence of order 8, which is fast (O(n)) and works in
 *  any build that provides the integer type seq_t.  The array
 *  `buf` always holds the last eight computed terms, with
 *  buf[0] = a(i-8) and buf[7] = a(i-1) when computing a(i).  */
seq_t a(idx_t n)
{
    /* initial eight terms */
    static const seq_t init[8] = {
        2,      /* a(1) */
        8,      /* a(2) */
        18,     /* a(3) */
        45,     /* a(4) */
        125,    /* a(5) */
        320,    /* a(6) */
        832,    /* a(7) */
        2197    /* a(8) */
    };

    if (n == 0) return 0;            /* behaviour below offset is irrelevant */
    if (n <= 8) return init[n-1];

    /* sliding window of the last eight terms */
    seq_t buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = init[i];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t next = 3*buf[7] - buf[6]
                   + 3*buf[5] - 9*buf[4]
                   + 3*buf[3] - buf[2]
                   + 3*buf[1] - buf[0];
        /* shift left */
        for (int j = 0; j < 7; ++j) buf[j] = buf[j+1];
        buf[7] = next;
    }
    return buf[7];
}
