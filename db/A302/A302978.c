/*
 * A302978  Chromatic invariant of the n-path complement graph.
 * https://oeis.org/A302978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302978 (chromatic invariant of the n‑path complement graph)
   Formula (n > 3):
      a(n) = Σ_{k=0..⌊n/2⌋} binomial(n‑k, k) * (‑1)^k * (n‑k‑2)!
   Initial values:
      a(1) = 1, a(2) = 2, a(3) = –1 */

/* No external functions are required; all operations use seq_t */
seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)(-1);

    /* n ≥ 4 from here on */
    idx_t N = n;

    /* Compute (N‑2)!  */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= N - 2; ++i)
        fact *= (seq_t)i;

    /* Binomial coefficient C(N, 0) = 1 */
    seq_t binom = (seq_t)1;
    seq_t sum   = (seq_t)0;
    int   sign  = 1;                     /* +1 or –1 */
    idx_t maxk  = N / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* Add the current term, with appropriate sign */
        if (sign > 0)
            sum += binom * fact;
        else
            sum -= binom * fact;

        /* If this was the last term, finish */
        if (k == maxk) break;

        /* Update binomial coefficient to C(N‑(k+1), k+1) */
        seq_t num1 = (seq_t)(N - 2 * k);
        seq_t den1 = (seq_t)(k + 1);
        binom = binom * num1 / den1;

        seq_t num2 = (seq_t)(N - 2 * k - 1);
        seq_t den2 = (seq_t)(N - k);
        binom = binom * num2 / den2;

        /* Update factorial to (N‑(k+1)‑2)! = (N‑k‑3)! */
        seq_t denom_fact = (seq_t)(N - k - 2);
        fact = fact / denom_fact;

        /* Toggle the sign for the next term */
        sign = -sign;
    }

    return sum;
}
