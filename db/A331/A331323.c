/*
 * A331323  a(n) = [x^n] (1 - 2*x)/(1 - 8*x + 4*x^2)^(3/2).
 * https://oeis.org/A331323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k*(k+1)*C(n+1,k+1)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Treat a(0) explicitly – the formula works for n = 0 as well,
       but we keep the special case for clarity and speed. */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = 0;                  /* running sum of all terms   */
    seq_t bin = (seq_t)(n + 1);     /* C(n+1, 1) initially      */
    seq_t pow3 = 1;                 /* 3^0 initially             */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = pow3 * (k+1) * bin * bin   (all seq_t operations) */
        seq_t term = pow3;
        term = term * (seq_t)(k + 1);
        term = term * bin;
        term = term * bin;

        sum = sum + term;

        /* Prepare values for next iteration */
        if (k == n) {
            break; /* loop finished */
        }

        /* Compute next binomial: C(n+1, k+2) = bin * (n - k) / (k + 2) */
        seq_t fact_num = (seq_t)(n - k);          /* (n+1 - (k+1)) = n - k */
        seq_t fact_den = (seq_t)(k + 2);
        bin = (bin * fact_num) / fact_den;

        pow3 = pow3 * 3; /* 3^(k+1) for next iteration */
    }

    return sum;
}
