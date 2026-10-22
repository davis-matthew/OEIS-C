/*
 * A383196  Expansion of e.g.f. (1/(1 - 3*x)^(1/3) - 1)^3 / 6.
 * https://oeis.org/A383196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((3^n)*n! + 3*(∏_{j=0}^{n-1} (3j+1) - ∏_{j=0}^{n-1}
 *            (3j+2)) ) / 6
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* a(0) = 0 by convention */

    seq_t fact = 1;     /* n!          */
    seq_t pow3 = 1;     /* 3^n         */
    seq_t A1 = 1;       /* ∏(3j+1)     */
    seq_t A2 = 1;       /* ∏(3j+2)     */

    for (idx_t i = 0; i < n; ++i) {
        seq_t inc = (seq_t)(i + 1);           /* factor for factorial          */
        fact *= inc;                          /* multiply by (i+1)             */

        pow3 *= (seq_t)3;                    /* multiply by 3                 */

        seq_t t1 = (seq_t)(3 * i + 1);        /* term for A1                  */
        seq_t t2 = (seq_t)(3 * i + 2);        /* term for A2                  */
        A1 *= t1;
        A2 *= t2;
    }

    /* numerator: 3^n * n! + 3*(A1 - A2)  */
    seq_t num = pow3 * fact;                /* 3^n * n!                      */
    num += A1 * (seq_t)3;                   /* add 3*A1                       */
    num -= A2 * (seq_t)3;                   /* subtract 3*A2 (no underflow)  */

    return num / (seq_t)6;                  /* divide by 6 (exact division)   */
}
