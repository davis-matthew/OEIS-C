/*
 * A297585  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1 or 4 neighboring 1s.
 * https://oeis.org/A297585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)
 *            +4*a(n-2)+23*a(n-3)+59*a(n-4)-6*a(n-5)-158*a(n-6)-137*a(n-7)-2*a(n-8)+54*a(n-9)+177*a(n-10)-230*a(n-11)-89*a(n-12)+168*a(n-13)-28*a(n-14)-37*a(n-15)+4*a(n-16)+2*a(n-17)
 *            for n>17 Initial values: a(1)=1, a(2)=20, a(3)=102, a(4)=254,
 *            a(5)=1017, a(6)=5263, a(7)=20771, a(8)=76340, a(9)=3...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n<1, return 0 */

    static const seq_t init[17] = {
        (seq_t)1ULL,      (seq_t)20ULL,      (seq_t)102ULL,      (seq_t)254ULL,
        (seq_t)1017ULL,   (seq_t)5263ULL,    (seq_t)20771ULL,    (seq_t)76340ULL,
        (seq_t)320326ULL, (seq_t)1353690ULL, (seq_t)5389975ULL,  (seq_t)21589173ULL,
        (seq_t)88747475ULL,(seq_t)362176212ULL,(seq_t)1462891858ULL,(seq_t)5938102840ULL,
        (seq_t)24195113809ULL
    };

    if (n <= 17)
        return init[n - 1];

    /* buffer holds a(k-1), a(k-2), ..., a(k-17) during iteration */
    seq_t buf[17];
    int i;
    for (i = 0; i < 17; ++i)
        buf[i] = init[16 - i];      /* buf[0] = a(17), buf[16] = a(1) */

    for (idx_t k = 18; k <= n; ++k) {
        seq_t val = 0;
        val += buf[0];                   /* +1  * a(k-1) */
        val += buf[1] * (seq_t)4;        /* +4  * a(k-2) */
        val += buf[2] * (seq_t)23;       /* +23 * a(k-3) */
        val += buf[3] * (seq_t)59;       /* +59 * a(k-4) */
        val -= buf[4] * (seq_t)6;        /* -6  * a(k-5) */
        val -= buf[5] * (seq_t)158;      /* -158 * a(k-6) */
        val -= buf[6] * (seq_t)137;      /* -137 * a(k-7) */
        val -= buf[7] * (seq_t)2;        /* -2   * a(k-8) */
        val += buf[8] * (seq_t)54;       /* +54  * a(k-9) */
        val += buf[9] * (seq_t)177;      /* +177 * a(k-10) */
        val -= buf[10] * (seq_t)230;     /* -230 * a(k-11) */
        val -= buf[11] * (seq_t)89;      /* -89  * a(k-12) */
        val += buf[12] * (seq_t)168;     /* +168 * a(k-13) */
        val -= buf[13] * (seq_t)28;      /* -28  * a(k-14) */
        val -= buf[14] * (seq_t)37;      /* -37  * a(k-15) */
        val += buf[15] * (seq_t)4;       /* +4   * a(k-16) */
        val += buf[16] * (seq_t)2;       /* +2   * a(k-17) */

        /* shift buffer: oldest entry (a(k-17)) discarded */
        for (i = 16; i > 0; --i)
            buf[i] = buf[i - 1];
        buf[0] = val;
    }

    return buf[0];                     /* buf[0] holds a(n) after loop */
}
