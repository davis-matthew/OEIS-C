/*
 * A354944  a(n) = Sum_{k=0..n} binomial(n,k)^3 * k! * (-n)^(n-k).
 * https://oeis.org/A354944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 * k! * (-n)^(n-k)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 8, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;   /* binomial(n,0)   */
    seq_t fact  = 1;   /* 0!              */
    seq_t neg_n = -(seq_t)n;
    seq_t pow   = 1;   /* (-n)^n          */

    for (idx_t i = 0; i < n; ++i)
        pow *= neg_n;

    /* term for k = 0 */
    seq_t term = binom;
    term = term * binom * binom;   /* binom^3 */
    term = term * fact;
    term = term * pow;
    sum += term;

    for (idx_t k = 0; k < n; ++k) {
        /* update to binomial(n,k+1) */
        binom = binom * (n - k);
        binom = binom / (k + 1);
        /* update factorial */
        fact = fact * (k + 1);
        /* update power: (-n)^(n-(k+1)) = pow / (-n) */
        pow = pow / neg_n;

        term = binom;
        term = term * binom * binom; /* binom^3 */
        term = term * fact;
        term = term * pow;
        sum += term;
    }
    return sum;
}
