/*
 * A374981  a(n) = (n^2 - 1)/6 - Sum_{t=0..n-1} [ A000217(t)/n ], where [ x ]
 *          means the fractional part of x here.
 * https://oeis.org/A374981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^2 - 1)/6 - Sum_{t=0..n-1} frac(A000217(t)/n) where
 *            A000217(t) = t(t+1)/2 and frac(x) = x - floor(x)
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes a(n) using integer arithmetic:
   a(n) = (((n^2 - 1)*n) - 6 * sum_{t=0}^{n-1} ( (t(t+1)/2) mod n )) / (6*n)
   This guarantees an exact integer result for all n ≥ 1. */
seq_t a(idx_t n)
{
    /* Safety check: the sequence is defined for n ≥ 1 */
    if (n == 0)
        return 0;

    idx_t t;
    seq_t sum_rem = 0;
    seq_t seq_n = (seq_t)n;

    /* Sum of remainders of triangular numbers modulo n */
    for (t = 0; t < n; ++t) {
        seq_t t_seq = (seq_t)t;
        seq_t T = (t_seq * (t_seq + (seq_t)1)) / 2;          /* t(t+1)/2 */
        sum_rem += T % seq_n;                                /* remainder mod n */
    }

    /* Compute numerator: (n^2-1)*n - 6*sum_rem */
    seq_t n_sq       = seq_n * seq_n;                        /* n^2   */
    seq_t numerator1 = (n_sq - (seq_t)1) * seq_n;            /* (n^2-1)*n */
    seq_t numerator2 = 6 * sum_rem;                          /* 6*sum_rem */
    seq_t numerator  = numerator1 - numerator2;

    /* Denominator: 6*n */
    seq_t denom = 6 * seq_n;

    /* Exact division yields the integer value a(n) */
    return numerator / denom;
}
