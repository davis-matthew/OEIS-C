/*
 * A386787  a(n) = n^4*sigma_7(n).
 * https://oeis.org/A386787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^4 * sigma_7(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 3181,
 *            big any n
 * Helpers:   power7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power7.h"

seq_t a(idx_t n)
{
    /* compute sigma_7(n) by enumerating divisors */
    seq_t sigma = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            sigma += power7(d1);
            idx_t j = n / i;
            if (j != i) {
                seq_t d2 = (seq_t)j;
                sigma += power7(d2);
            }
        }
    }

    /* compute n^4 */
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;
    seq_t n4 = n2 * n2;

    return n4 * sigma;
}
