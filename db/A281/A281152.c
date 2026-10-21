/*
 * A281152  Least number k such that Sum_{j=k..k+n-1}{j} = Sum_{j=k+n..t}{j},
 *          for some t >= k+n.
 * https://oeis.org/A281152
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the least k such that Sum_{i=0}^{n-1}(k+i) =
 *            Sum_{i=0}^{m-1}(k+n+i) for some m>=1. a(n) is the minimum k over
 *            all 1 <= d = n-m <= n-1 for which k = (2n^2 - 4nd + d^2 +
 *            d)/(2d) is an integer
 * Range:     u8 n <= 12, u16 n <= 58, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* below offset – undefined, return 0 */

    seq_t best = 0;
    bool found = false;
    seq_t n_seq = (seq_t)n;

    for (idx_t d = 1; d < n; ++d)
    {
        seq_t d_seq = (seq_t)d;
        /* N = 2n^2 - 4nd + d^2 + d */
        seq_t N = (seq_t)2 * n_seq * n_seq
                - (seq_t)4 * n_seq * d_seq
                + d_seq * d_seq
                + d_seq;

        seq_t div = (seq_t)2 * d_seq;
        if (N % div == 0)
        {
            seq_t k = N / div;
            if (k > 0 && (!found || k < best))
            {
                best = k;
                found = true;
            }
        }
    }

    /* for all n >= 2 a solution exists, but return 0 if not found */
    return found ? best : (seq_t)0;
}
