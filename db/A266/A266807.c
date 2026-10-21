/*
 * A266807  Coefficient of x^3 in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(6),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5),
 *            with a(0)=2, a(1)=-90, a(2)=-166, a(3)=-2166, a(4)=-12010
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 10, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Predefined base values.  They are stored as seq_t literals. */
    static const seq_t init[5] = {
        (seq_t)2,
        (seq_t)-90,
        (seq_t)-166,
        (seq_t)-2166,
        (seq_t)-12010
    };

    /* Handle the first five indices directly. */
    if (n < 5) {
        return init[(int)n];
    }

    /* Sliding window of the last five terms.  We avoid dynamic allocation
       and compute the sequence iteratively, which is O(n) and uses only
       constant additional space. */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t an = a4;        /* placeholder */

    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)5 * a4 + (seq_t)15 * a3 - (seq_t)15 * a2 - (seq_t)5 * a1 + a0;
        /* shift the window forward by one position */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
