/*
 * A263712  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -3 to 3 places and every four consecutive
 *          elements having its maximum within 4 of its minimum.
 * https://oeis.org/A263712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = a(n-1) + a(n-3) - a(n-4) + 2*a(n-5)
 *            - a(n-6) + a(n-7) for n > 18 Initial values: a(1) = 1, a(2) = 2,
 *            a(3) = 6, a(4) = 24, a(5) = 78, a(6) = 60, a(7) = 50, a(8) = 52,
 *            a(9) = 70, a(10)=116, a(11)=184, a(12)=292, a(13)=449,
 *            a(14)=649, a(15)=969, a(16)=1429, a(17)=2137, a(...
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 52, u64 n <= 107,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init[19] = {
    0,1,2,6,24,78,60,50,52,70,116,184,292,449,649,969,1429,2137,3238
};

seq_t a(idx_t n)
{
    if (n <= 18)
        return init[n];

    /* buf[i] holds a(current-7 + i) for i=0..6.
       At the start we need values for a(12) .. a(18). */
    seq_t buf[7];
    buf[0] = init[12];  /* a(12) */
    buf[1] = init[13];  /* a(13) */
    buf[2] = init[14];  /* a(14) */
    buf[3] = init[15];  /* a(15) */
    buf[4] = init[16];  /* a(16) */
    buf[5] = init[17];  /* a(17) */
    buf[6] = init[18];  /* a(18) */

    seq_t val = 0;
    for (idx_t i = 19; i <= n; ++i) {
        /* recurrence */
        val = buf[6] + buf[4] - buf[3] + buf[2] + buf[2] - buf[1] + buf[0];

        /* slide window */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = val;
    }
    return val;
}
