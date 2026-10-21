/*
 * A293469  a(n) = Sum_{k=0..n} (2*k-1)!!*binomial(2*n-k, n).
 * https://oeis.org/A293469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (2*k-1)!! * binomial(2*n-k, n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;                      /* running total */
    seq_t f   = 1;                       /* (2*k-1)!!, starts at k = 0 */
    seq_t B   = binom(2*n, n);           /* C(2n, n), starts at k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        sum += f * B;                    /* add current term */

        if (k == n) break;              /* finished last term */

        /* update (2*k-1)!! to (2*(k+1)-1)!! */
        f = f * ((seq_t)(2*k) + 1);

        /* update B to C(2n-(k+1), n) */
        seq_t num = n - k;
        seq_t den = (seq_t)(2*n) - k;
        B = B * num / den;
    }
    return sum;
}
