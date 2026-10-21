/*
 * A247798  n-th positive integer relatively prime to 2*n - 1.
 * https://oeis.org/A247798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that is coprime to (2*n-1)
 * Range:     u8 n <= 135, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n < 1; return 0 */
        return (seq_t)0;

    seq_t M = ((seq_t)n << 1) - 1;   /* M = 2*n - 1, as seq_t */

    idx_t seen = 0;
    seq_t k = 1;

    for (;;) {
        if (gcd(k, M) == (seq_t)1) {   /* count coprime numbers */
            ++seen;
            if (seen == n)
                return k;              /* nth coprime found */
        }
        ++k;
    }
}
