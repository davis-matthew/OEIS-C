/*
 * A267064  Coefficient of x^5 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(2)+sqrt(3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 34*a(n-1) + 714*a(n-2) - 4641*a(n-3) - 12376*a(n-4) +
 *            12376*a(n-5) + 4641*a(n-6) - 714*a(n-7) - 34*a(n-8) + a(n-9),
 *            with a(0)=−68, a(1)=−5760, a(2)=−35252, a(3)=−4744764,
 *            a(4)=−160222784, a(5)=−8602304988, a(6)=−384492157220,
 *            a(7)=−18412926914112, a(8)=−858719581400084
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 4, i64 n <= 10, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A267064. */
seq_t a(idx_t n)
{
    /* initial nine terms supplied as constants. */
    static const seq_t init[9] = {
        (seq_t)-68,
        (seq_t)-5760,
        (seq_t)-35252,
        (seq_t)-4744764,
        (seq_t)-160222784,
        (seq_t)-8602304988,
        (seq_t)-384492157220,
        (seq_t)-18412926914112,
        (seq_t)-858719581400084
    };

    /* any n < 9 directly from the table */
    if (n < 9)
        return init[n];

    /* buffer holding the last nine computed values:
       buffer[0] = a(k-9), buffer[8] = a(k-1) for current k. */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i)
        buf[i] = init[i];

    /* iterate from k = 9 up to n, filling the recurrence. */
    for (idx_t k = 9; k <= n; ++k) {
        seq_t val =  (seq_t)34 * buf[8]   /* a(k-1)   */
                  + (seq_t)714 * buf[7]  /* a(k-2)   */
                  - (seq_t)4641 * buf[6] /* a(k-3)   */
                  - (seq_t)12376 * buf[5]/* a(k-4)   */
                  + (seq_t)12376 * buf[4]/* a(k-5)   */
                  + (seq_t)4641 * buf[3] /* a(k-6)   */
                  - (seq_t)714 * buf[2]  /* a(k-7)   */
                  - (seq_t)34  * buf[1]  /* a(k-8)   */
                  + buf[0];             /* a(k-9)   */

        /* shift buffer left: discard a(k-9), keep a(k-8)..a(k-1) */
        for (int i = 0; i < 8; ++i)
            buf[i] = buf[i + 1];
        buf[8] = val;  /* new term a(k) */
    }

    return buf[8]; /* a(n) */
}
