/*
 * A308733  Sum of the smallest parts of the partitions of n into 4 parts.
 * https://oeis.org/A308733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-4)-3*a(n-5)-a(n-6)+a(n-8)+3*a(n-9) *
 *            -a(n-10)-a(n-12)-a(n-13)+a(n-14) * with initial values * a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=1, a(6)=2, * a(7)=3,
 *            a(8)=6, a(9)=7, a(10)=11, a(11)=14, a(12)=21, * a(13)=25,
 *            a(14)=34
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for a(0) .. a(14) */
    static const seq_t init[15] = {
        0, 0, 0, 0, 1, 1, 2, 3, 6, 7, 11, 14, 21, 25, 34
    };

    /* For n < 15 simply return the pre‑computed value. */
    if (n < 15)
        return init[n];

    /* We keep the last 15 values a(i-14)..a(i-1) in a circular buffer. */
    seq_t buf[15];
    for (int i = 0; i < 15; ++i)
        buf[i] = init[i];   /* buf[0]=a(0), buf[14]=a(14) */

    /* Compute a(n) iteratively. After each iteration we shift the buffer so
     * that buf[0] holds a(i-13), buf[14] holds a(i), where i is the current
     * index. */
    for (int i = 15; i <= (int)n; ++i) {
        seq_t next =      buf[14]          /* a(i-1)    */
                     + buf[13]          /* a(i-2)    */
                     + buf[11]          /* a(i-4)    */
                     - 3 * buf[10]      /* 3*a(i-5)  */
                     - buf[9]           /* a(i-6)    */
                     + buf[7]           /* a(i-8)    */
                     + 3 * buf[6]       /* 3*a(i-9)  */
                     - buf[5]           /* a(i-10)   */
                     - buf[3]           /* a(i-12)   */
                     - buf[2]           /* a(i-13)   */
                     + buf[1];          /* a(i-14)   */

        /* Shift buf left by one position */
        for (int j = 0; j < 14; ++j)
            buf[j] = buf[j + 1];
        buf[14] = next;       /* buf[14] now holds a(i) */
    }

    return buf[14];
}
