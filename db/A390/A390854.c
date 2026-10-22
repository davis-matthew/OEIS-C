/*
 * A390854  a(n) = Sum_{k=0..n} (-1)^k * binomial(n+2*k-1,n-k) *
 *          Fibonacci(k+1).
 * https://oeis.org/A390854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 5*a(n-1) - 11*a(n-2) + 17*a(n-3) - 14*a(n-4) + 6*a(n-5) -
 *            a(n-6) for n > 6 with initial values a(0)=1, a(1)=-1, a(2)=-1,
 *            a(3)=3, a(4)=10, a(5)=14, a(6)=19
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 24, i64 n <= 47, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First seven terms as given in the OEIS entry.  */
    static const seq_t init[7] = {1, -1, -1, 3, 10, 14, 19};

    if (n < 7)               /* return the pre‑computed value */
        return init[n];

    /* a[0] .. a[5] hold a(n-6) .. a(n-1) respectively.  */
    seq_t a[6];
    a[0] = init[1];   /* a(1)  = -1  */
    a[1] = init[2];   /* a(2)  = -1  */
    a[2] = init[3];   /* a(3)  =  3  */
    a[3] = init[4];   /* a(4)  = 10  */
    a[4] = init[5];   /* a(5)  = 14  */
    a[5] = init[6];   /* a(6)  = 19  */

    seq_t next = 0;
    for (idx_t i = 7; i <= n; ++i) {
        /* Apply the recurrence. */
        next = (seq_t)5 * a[5]
             - (seq_t)11 * a[4]
             + (seq_t)17 * a[3]
             - (seq_t)14 * a[2]
             + (seq_t)6  * a[1]
             - a[0];

        /* Shift the window: drop a[0], shift all others left, and append next. */
        a[0] = a[1];
        a[1] = a[2];
        a[2] = a[3];
        a[3] = a[4];
        a[4] = a[5];
        a[5] = next;
    }
    return next;   /* a(n) */
}
