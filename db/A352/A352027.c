/*
 * A352027  a(n) = binomial(2*n-1,n) - n*(n-1) - 1.
 * https://oeis.org/A352027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-1, n) - n*(n-1) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For indices below the offset (n < 1) return 0 */
    if (n < 1)
        return (seq_t)0;

    /* Compute the required binomial coefficient */
    seq_t bin = binomial(2 * n - 1, n);

    /* Compute n*(n-1) using seq_t arithmetic */
    seq_t sub = (seq_t)n * ((seq_t)n - 1);

    /* Result is bin - sub - 1 */
    return bin - sub - (seq_t)1;
}
