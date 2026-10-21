/*
 * A277387  Number of nonnegative solutions of a certain system of linear
 *          Diophantine equations depending on an even parameter.
 * https://oeis.org/A277387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(3+2*n+n^2+n^3+2*n^4)/18
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 57, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* below offset behaviour is irrelevant */

    /* Core variables */
    seq_t a = (seq_t)n;           /* n            */
    seq_t b = a + 1;              /* n+1          */
    seq_t t = (seq_t)3;           /* start of polynomial */

    /* Compute higher powers of n, relying on seq_t to hold them safely */
    seq_t n2 = a * a;             /* n^2          */
    seq_t n3 = n2 * a;            /* n^3          */
    seq_t n4 = n3 * a;            /* n^4          */

    /* Build the polynomial part */
    t += (seq_t)2 * a;            /* +2*n         */
    t += n2;                      /* +n^2         */
    t += n3;                      /* +n^3         */
    t += (seq_t)2 * n4;           /* +2*n^4       */

    /* Divide the product by 18 by cancelling factors early.
       18 = 2 * 3 * 3. We divide by 2 first, then by 3 twice. */
    if ((a & 1) == 0)              /* a even, divide a by 2 */
        a /= 2;
    else
        b /= 2;                    /* else b even, divide b by 2 */

    /* Divide by 3 two times, using any of the three factors that
       still contain a factor of 3. */
    for (int i = 0; i < 2; ++i) {
        if ((a % 3) == 0)
            a /= 3;
        else if ((b % 3) == 0)
            b /= 3;
        else if ((t % 3) == 0)
            t /= 3;
        /* In the mathematically correct sequence the product is
           always divisible by 18, so one of the factors must contain
           the needed factor of 3. */
    }

    /* Multiply the three remaining factors: the result fits in seq_t
       if it fits in the target type, because each factor is now
       reduced by the necessary divisibility. */
    seq_t result = a * b;
    result *= t;
    return result;
}
