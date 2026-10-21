/*
 * A266805  Coefficient of x in the minimal polynomial of the continued
 *          fraction [1^n,sqrt(6),1,1,...], where 1^n means n ones.
 * https://oeis.org/A266805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5)
 *            Initial values: a(0) = -14, a(1) = -90, a(2) = -722, a(3) =
 *            -4830, a(4) = -33554
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[5] = {
        (seq_t)-14,
        (seq_t)-90,
        (seq_t)-722,
        (seq_t)-4830,
        (seq_t)-33554
    };

    /* Handle indices below the recurrence order */
    if (n < 5) {
        return init[n];
    }

    /* Variables hold the last five computed terms:
       a_nm5 = a(n-5), a_nm4 = a(n-4), ..., a_nm1 = a(n-1)  */
    seq_t a_nm5 = init[0];
    seq_t a_nm4 = init[1];
    seq_t a_nm3 = init[2];
    seq_t a_nm2 = init[3];
    seq_t a_nm1 = init[4];

    idx_t i;
    for (i = 5; i <= n; ++i) {
        /* a(n) = 5*a(n-1) + 15*a(n-2) - 15*a(n-3) - 5*a(n-4) + a(n-5) */
        seq_t a = (seq_t)5*a_nm1 + (seq_t)15*a_nm2 - (seq_t)15*a_nm3
                  - (seq_t)5*a_nm4 + a_nm5;

        /* Shift the window for the next iteration */
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a;
    }

    return a_nm1; /* This is a(n) */
}
