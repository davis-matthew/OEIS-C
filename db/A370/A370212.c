/*
 * A370212  Coefficient of x^n in the expansion of ( (1+x) / (1-x^3) )^n.
 * https://oeis.org/A370212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/3⌋} binomial(n+k-1,k) * binomial(n,3k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    seq_t t1   = 1;            /* binomial(n + k - 1, k) */
    seq_t t2   = 1;            /* binomial(n, 3k)   */
    idx_t  m   = 0;            /* current value of 3k */
    idx_t  maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        if (k > 0) {
            /* t1 = t1 * (n + k - 1) / k  */
            t1 = t1 * (n + k - 1) / k;

            /* Update t2 to binomial(n, 3k) by three successive steps */
            for (int inc = 0; inc < 3; ++inc) {
                ++m;                        /* m = 3k */
                t2 = t2 * (n - (m - 1)) / m;
            }
        }
        sum += t1 * t2;
    }
    return sum;
}
