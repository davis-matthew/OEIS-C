/*
 * A267066  Coefficient of x^6 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(2)+sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 34*a(n-1) + 714*a(n-2) - 4641*a(n-3) - 12376*a(n-4) +
 *            12376*a(n-5) + 4641*a(n-6) - 714*a(n-7) - 34*a(n-8) + a(n-9)
 *            Initial values: a(0)=4, a(1)=−560, a(2)=−952, a(3)=−303372,
 *            a(4)=−8139896, a(5)=−481544336, a(6)=−20771606140,
 *            a(7)=−1008539866512, a(8)=−46789454179352 This implementation
 *            co...
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 5, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    static const seq_t init[9] = {
        (seq_t)4,
        (seq_t)-560,
        (seq_t)-952,
        (seq_t)-303372,
        (seq_t)-8139896,
        (seq_t)-481544336,
        (seq_t)-20771606140,
        (seq_t)-1008539866512,
        (seq_t)-46789454179352
    };

    if (n < 9)
        return init[n];

    /* Sliding window of the 9 previous terms */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i)
        buf[i] = init[i];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t next =   (seq_t)34  * buf[8]
                     + (seq_t)714 * buf[7]
                     - (seq_t)4641 * buf[6]
                     - (seq_t)12376 * buf[5]
                     + (seq_t)12376 * buf[4]
                     + (seq_t)4641 * buf[3]
                     - (seq_t)714 * buf[2]
                     - (seq_t)34  * buf[1]
                     +          buf[0];

        /* Shift buffer left by one */
        for (int j = 0; j < 8; ++j)
            buf[j] = buf[j + 1];
        buf[8] = next;
    }

    return buf[8];
}
