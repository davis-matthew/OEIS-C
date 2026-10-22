/*
 * A307905  Coefficient of x^n in (1 + n*x + x^3)^n.
 * https://oeis.org/A307905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} n^(n-3*k) * binomial(n,3*k) *
 *            binomial(3*k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t max_k = n / 3;
    seq_t pow_cur = 1;
    /* compute n^n */
    for (idx_t i = 0; i < n; ++i)
        pow_cur *= (seq_t)n;

    seq_t n_cube = (seq_t)n * (seq_t)n * (seq_t)n;   /* n^3 */
    seq_t sum = 0;

    seq_t binom_n = 1;   /* C(n,0) */
    seq_t binom_3k = 1;  /* C(0,0) */

    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t term = pow_cur * binom_n;
        term *= binom_3k;
        sum += term;

        /* prepare for next k */
        if (k == max_k)
            break;

        /* update C(n, 3*(k+1)) from C(n, 3*k) */
        idx_t r = 3 * (k + 1);                 /* new r = 3*k */
        seq_t num = (seq_t)(n - r + 1) *
                    (seq_t)(n - r + 2) *
                    (seq_t)(n - r + 3);
        seq_t den = (seq_t)(r - 2) *
                    (seq_t)(r - 1) *
                    (seq_t)(r);
        binom_n = binom_n * num / den;

        /* update C(3*(k+1), k+1) from C(3*k, k) */
        idx_t pk = k + 1; /* current k after increment */
        seq_t numT = (seq_t)(3 * pk) *
                     (seq_t)(3 * pk - 1) *
                     (seq_t)(3 * pk - 2);
        seq_t denT = (seq_t)(pk) *
                     (seq_t)(2 * pk) *
                     (seq_t)(2 * pk - 1);
        binom_3k = binom_3k * numT / denT;

        /* update power: divide by n^3 to get next n^(n-3*(k+1)) */
        pow_cur /= n_cube;
    }

    return sum;
}
