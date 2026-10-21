/*
 * A250082  Number of length 1+5 0..n arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n^5 + 10*n^4 + 15*n^3 + (27*n^2 + 13*n)/2 + 1 (for n >=
 *            2) a(1) = 49 Corresponds to OEIS A250082
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 67, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 1 to match the OEIS value */
    if (n == 1) {
        return (seq_t)49;
    }

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;            /* n^2 */
    seq_t N3 = N2 * N;           /* n^3 */
    seq_t N4 = N3 * N;           /* n^4 */
    seq_t N5 = N4 * N;           /* n^5 */

    /* Compute (27*n^2 + 13*n)/2 exactly */
    seq_t temp     = (seq_t)27 * N2 + (seq_t)13 * N;
    seq_t term45   = temp / 2;

    /* Assemble the polynomial */
    seq_t result = N5 * (seq_t)3
                 + N4 * (seq_t)10
                 + N3 * (seq_t)15
                 + term45
                 + (seq_t)1;

    return result;
}
