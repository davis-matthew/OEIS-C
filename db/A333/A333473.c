/*
 * A333473  a(n) = [x^n] ( S(x/(1 + x)) )^n, where S(x) = (1 - x - sqrt(1 -
 *          6*x + x^2))/(2*x) is the o.g.f. of the large Schröder numbers
 *          A006318.
 * https://oeis.org/A333473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(n+2*k-1, 2*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result   = 0;   /* accumulating sum */
    seq_t binom_nk = 1;   /* binomial(n,k)  */
    seq_t B        = 1;   /* binomial(n+2*k-1, 2*k) */

    for (idx_t k = 0; k <= n; ++k) {
        result += binom_nk * B;

        if (k == n)          /* last term processed */
            break;

        /* Update binomial(n,k+1) from binomial(n,k) */
        binom_nk = binom_nk * (seq_t)(n - k) / (seq_t)(k + 1);

        /* Update B_{k+1} from B_k
           B_{k+1} = B_k * (n+2k) * (n+2k+1) / ((2k+1)*(2k+2))  */
        idx_t kk   = k + 1;
        seq_t num1 = (seq_t)(n + 2 * kk - 2);   /* n + 2k      */
        seq_t num2 = (seq_t)(n + 2 * kk - 1);   /* n + 2k + 1  */
        seq_t den1 = (seq_t)(2 * kk - 1);       /* 2k + 1      */
        seq_t den2 = (seq_t)(2 * kk);           /* 2k + 2      */

        B = B * num1;
        B = B / den1;
        B = B * num2;
        B = B / den2;
    }

    return result;
}
