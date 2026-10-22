/*
 * A362320  a(n) = n! * Sum_{k=0..floor(n/5)} (-n/5)^k / (k! * (n-5*k)!).
 * https://oeis.org/A362320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/5)} (-n/5)^k / (k! * (n-5*k)!)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 12, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* term_k = a_k / n! for the current k.  term_0 = 1. */
    seq_t sum  = 1;     /* a(0) = 1 for all n >= 0 */
    seq_t term = 1;     /* term_0 */
    idx_t maxk = n / 5; /* floor(n/5) */

    for (idx_t k = 0; k < maxk; ++k)
    {
        /* Compute prod = product_{j=0}^{4} (n - 5*k - j)   */
        seq_t prod = 1;
        for (int j = 0; j < 5; ++j)
            prod *= (seq_t)(n - 5*k - j);

        /* The product of 5 consecutive integers is divisible by 5.  */
        prod /= (seq_t)5;

        /* term_{k+1} = - term_k * prod * n / (k+1)  */
        seq_t numer = term * prod;
        numer *= (seq_t)n;
        numer /= (seq_t)(k + 1);

        term = -numer;
        sum += term;
    }
    return sum;
}
