/*
 * A267083  Coefficient of x^5 in the minimal polynomial of the continued
 *          fraction [1^n,2^(1/3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 13*a(n-1) + 104*a(n-2) - 260*a(n-3) - 260*a(n-4) +
 *            104*a(n-5) + 13*a(n-6) - a(n-7) Initial values: a(0)=3,
 *            a(1)=-15, a(2)=-111, a(3)=-1419, a(4)=-32847, a(5)=-549633,
 *            a(6)=-10161885, a(7)=-180368529, a(8)=-3250331151
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 15, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    static const seq_t base[9] = {
        (seq_t)3,
        (seq_t)-15,
        (seq_t)-111,
        (seq_t)-1419,
        (seq_t)-32847,
        (seq_t)-549633,
        (seq_t)-10161885,
        (seq_t)-180368529,
        (seq_t)-3250331151
    };

    if (n < 9)
        return base[n];

    /* Array holds last 7 terms: a(i-7) .. a(i-1) during the loop */
    seq_t last[7];
    last[0] = base[2]; /* a(2) */
    last[1] = base[3]; /* a(3) */
    last[2] = base[4]; /* a(4) */
    last[3] = base[5]; /* a(5) */
    last[4] = base[6]; /* a(6) */
    last[5] = base[7]; /* a(7) */
    last[6] = base[8]; /* a(8) */

    seq_t val = 0; /* will hold a(i) for current i */

    for (idx_t i = 9; i <= n; ++i) {
        val = 13 * last[6]
            + 104 * last[5]
            - 260 * last[4]
            - 260 * last[3]
            + 104 * last[2]
            + 13  * last[1]
            -      last[0];

        /* shift window for next iteration */
        for (idx_t j = 0; j < 6; ++j)
            last[j] = last[j + 1];
        last[6] = val;
    }

    return val;
}
