/*
 * A244048  Antisigma(n) minus the sum of remainders of n mod k, for k =
 *          1,2,3,...,n.
 * https://oeis.org/A244048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{n-1} (σ(m)-m) = Σ_{d=1}^{n-1} d·⌊(n-1)/d⌋ –
 *            (n-1)·n/2 (sum of proper divisors of all integers < n)
 * Range:     u8 n <= 30, u16 n <= 451, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9933 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t sum_sigma_upto(idx_t N)
{
    seq_t res = 0;
    idx_t d = 1;
    while (d <= N) {
        idx_t q = N / d;            /* floor(N/d) */
        idx_t last = N / q;         /* largest d with same quotient q */
        /* sum of integers from d to last */
        seq_t cnt   = (seq_t)(last - d + 1);
        seq_t sum_d = ((seq_t)d + (seq_t)last) * cnt / 2;
        res += sum_d * (seq_t)q;
        d = last + 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1)=0 by definition */
    idx_t N = n - 1;
    seq_t sigma_sum = sum_sigma_upto(N);

    seq_t nseq = (seq_t)n;
    seq_t n1    = (seq_t)N;        /* n-1 */

    /* (n-1)*n/2 */
    seq_t tri = n1 * nseq / 2;

    return sigma_sum - tri;
}
