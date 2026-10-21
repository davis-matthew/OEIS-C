/*
 * A298624  Number of nX3 0..1 arrays with every element equal to 2, 3, 4, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1)+5*a(n-2)-27*a(n-3)-26*a(n-4)+79*a(n-5)+33*a(n-6)
 *            * -168*a(n-7)+31*a(n-8)+142*a(n-9)-40*a(n-10)-8*a(n-11) for n >
 *            12 * Initial values: a(1)=0, a(2)=3, a(3)=8, a(4)=26, a(5)=138,
 *            a(6)=611, * a(7)=2880, a(8)=13727, a(9)=65070, a(10)=310816, *
 *            a(11)=1482870, a(12)=7081007
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fixed initial terms */
    static const seq_t init[12] = {
        (seq_t)0,      /* a(1) */
        (seq_t)3,      /* a(2) */
        (seq_t)8,      /* a(3) */
        (seq_t)26,     /* a(4) */
        (seq_t)138,    /* a(5) */
        (seq_t)611,    /* a(6) */
        (seq_t)2880,   /* a(7) */
        (seq_t)13727,  /* a(8) */
        (seq_t)65070,  /* a(9) */
        (seq_t)310816, /* a(10) */
        (seq_t)1482870,/* a(11) */
        (seq_t)7081007 /* a(12) */
    };

    if (n <= 12) {
        return init[n - 1];
    }

    /* Buffer holding the last 12 computed values:
     * buf[0]  = a(k-12), buf[1] = a(k-11), ..., buf[11] = a(k-1) */
    seq_t buf[12];
    for (int i = 0; i < 12; ++i) {
        buf[i] = init[i];
    }

    for (idx_t k = 13; k <= n; ++k) {
        /* Compute a(k) from the recurrence using the buffer */
        seq_t val =
            (seq_t)5  * buf[11]
          + (seq_t)5  * buf[10]
          - (seq_t)27 * buf[9]
          - (seq_t)26 * buf[8]
          + (seq_t)79 * buf[7]
          + (seq_t)33 * buf[6]
          - (seq_t)168* buf[5]
          + (seq_t)31 * buf[4]
          + (seq_t)142* buf[3]
          - (seq_t)40 * buf[2]
          - (seq_t)8  * buf[1];

        /* Shift buffer left by one to make room for the new value */
        for (int i = 0; i < 11; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[11] = val;   /* Now buf[11] holds a(k) */
    }

    return buf[11];
}
