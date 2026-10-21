/*
 * A267081  Coefficient of x^3 in the minimal polynomial of the continued
 *          fraction [1^n,2^(1/3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -9, a(1) = 35, a(2) = -969, a(3) = -14359, a(4) =
 *            -279261, a(5) = -4862231, a(6) = -88270665. For n > 6: a(n) =
 *            10*a(n-1) + 135*a(n-2) + 135*a(n-3) + 10*a(n-4) - a(n-5)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 14, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values a(0) … a(6)   */
    static const seq_t init[7] = { -9, 35, -969, -14359, -279261, -4862231, -88270665 };
    if (n <= 6) return init[n];

    /* sliding window of the five previous terms:
       a_nm5 = a(n-5), a_nm4 = a(n-4), a_nm3 = a(n-3),
       a_nm2 = a(n-2), a_nm1 = a(n-1)   */
    seq_t a_nm5 = init[2];   /* a(2)  */
    seq_t a_nm4 = init[3];   /* a(3)  */
    seq_t a_nm3 = init[4];   /* a(4)  */
    seq_t a_nm2 = init[5];   /* a(5)  */
    seq_t a_nm1 = init[6];   /* a(6)  */
    seq_t a_next;

    for (idx_t i = 7; i <= n; ++i) {
        a_next = (seq_t)10 * a_nm1
               + (seq_t)135 * a_nm2
               + (seq_t)135 * a_nm3
               + (seq_t)10 * a_nm4
               - a_nm5;
        /* shift the window */
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_next;
    }
    return a_next;
}
