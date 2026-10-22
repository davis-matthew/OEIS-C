/*
 * A394490  Expansion of e.g.f. (1/x) * Series_Reversion( x/(1 + 5*x)^(3/5) ).
 * https://oeis.org/A394490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Π_{k=0}^{n-1} (3*(n+1) - 5k) / (n+1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from         a(n) = 5^n * n! * binomial(3*(n+1)/5 , n)/(n+1)   */

static inline seq_t seq_gcd_local(seq_t a, seq_t b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t nplus1 = (seq_t)(n + 1);
    seq_t denom  = nplus1;   /* keeps the factor (n+1) to be cancelled   */
    seq_t ans    = 1;        /* accumulates the product                   */

    for (idx_t k = 0; k < n; ++k) {
        seq_t num = 3 * nplus1 - 5 * (seq_t)k;   /* 3*(n+1) - 5k          */

        /* cancel common factors with the remaining denominator   */
        seq_t g = seq_gcd_local(num, denom);
        if (g != 0) {
            num   /= g;
            denom /= g;
        }

        /* multiply the reduced numerator into the answer       */
        ans *= num;

        /* further cancel any remaining factors of the denominator */
        if (denom != 1) {
            seq_t g2 = seq_gcd_local(ans, denom);
            if (g2 != 0) {
                ans   /= g2;
                denom /= g2;
            }
        }
    }

    /* any leftover denominator factor (should be 1) is removed */
    if (denom != 1)
        ans /= denom;

    return ans;
}
