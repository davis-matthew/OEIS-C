/*
 * A370183  Coefficient of x^n in the expansion of ( (1+x) * (1+x+x^3) )^n.
 * https://oeis.org/A370183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n,k) * binomial(2*n-k,
 *            n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute binomial(2n, n) for n >= 0 */
static seq_t binom_2n_n(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* res = res * (n + i) / i; multiply first then divide */
        res = res * (n + i);
        res = res / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }

    const idx_t limit = n / 3;          /* floor(n/3) */
    seq_t sum = 0;

    /* C(n,0) = 1 */
    seq_t Cnk = 1;

    /* B(0) = binomial(2n, n) */
    seq_t Bk = binom_2n_n(n);
    sum += Cnk * Bk;                    /* term for k = 0 */

    for (idx_t k = 0; k < limit; ++k) {
        /* Update C(n,k+1) */
        Cnk = Cnk * (n - k) / (k + 1);

        /* Update B(k+1) using the derived ratio */
        seq_t b = n - 3 * k;                 /* b = n - 3k  */
        seq_t a_val = 2 * n - k;             /* a = 2n - k  */
        seq_t denom = a_val * (a_val - b + 1) * (a_val - b + 2);
        seq_t numer = b * (b - 1) * (b - 2);

        Bk = Bk * numer / denom;

        sum += Cnk * Bk;                    /* term for k+1 */
    }

    return sum;
}
