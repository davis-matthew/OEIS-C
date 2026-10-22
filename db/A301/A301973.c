/*
 * A301973  a(n) = (n^2 - 3*n + 6)*binomial(n+2,3)/4.
 * https://oeis.org/A301973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - 3*n + 6)*binomial(n+2,3)/4
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;                     /* cast index to seq_t */
    seq_t diff = nn*nn - (seq_t)3*nn + 6;    /* n^2 - 3n + 6 */
    seq_t bin  = binomial(n + 2, 3);         /* binomial(n+2,3) */
    return (diff * bin) / 4;                 /* divide by 4 */
}
