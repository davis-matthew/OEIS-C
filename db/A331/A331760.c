/*
 * A331760  a(n) = A115004(2n)/4.
 * https://oeis.org/A331760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A115004(2*n)/4 = n^2 + (Sum_{i=2..2n}
 *            (2*n+1-i)*(4*n+2-i)*phi(i))/4
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 288, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1, but guard against 0 */

    seq_t nSeq = (seq_t)n;
    seq_t result = nSeq * nSeq;   /* n^2 */

    idx_t limit = 2 * n;
    seq_t sum = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        seq_t phi_i = phi(i);                    /* phi(i) */
        seq_t term = ( ( (seq_t)(2 * n + 1 - i) *
                         (seq_t)(4 * n + 2 - i) ) * phi_i );
        sum += term;
    }

    result += sum / (seq_t)4;
    return result;
}
