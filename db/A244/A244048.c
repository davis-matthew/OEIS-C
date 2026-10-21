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
 * Helpers:   sum_sigma_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum_sigma_upto.h"

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
