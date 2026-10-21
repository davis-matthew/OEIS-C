/*
 * A249884  Number of length 1+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) (for n > 8), with a(1)=35,
 *            a(2)=805, a(3)=7420, a(4)=40740, a(5)=161315, a(6)=510965,
 *            a(7)=1377040, a(8)=3284400
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial eight terms (a(1) through a(8)) */
    static const seq_t init[8] = {
        (seq_t)35,
        (seq_t)805,
        (seq_t)7420,
        (seq_t)40740,
        (seq_t)161315,
        (seq_t)510965,
        (seq_t)1377040,
        (seq_t)3284400
    };

    if (n == 0)
        return (seq_t)0;          /* value for indices below the offset; unusued */

    if (n <= 8)
        return init[n - 1];

    /* Use a sliding window of the last eight values */
    seq_t a1 = init[0];  /* a(n-8)  */
    seq_t a2 = init[1];  /* a(n-7)  */
    seq_t a3 = init[2];  /* a(n-6)  */
    seq_t a4 = init[3];  /* a(n-5)  */
    seq_t a5 = init[4];  /* a(n-4)  */
    seq_t a6 = init[5];  /* a(n-3)  */
    seq_t a7 = init[6];  /* a(n-2)  */
    seq_t a8 = init[7];  /* a(n-1)  */

    for (idx_t i = 9; i <= n; ++i) {
        seq_t t = (seq_t)8  * a8
                - (seq_t)28 * a7
                + (seq_t)56 * a6
                - (seq_t)70 * a5
                + (seq_t)56 * a4
                - (seq_t)28 * a3
                + (seq_t)8  * a2
                - a1;
        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = a8;
        a8 = t;
    }
    return a8;
}
