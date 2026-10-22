/*
 * A368561  a(n) = Sum_{k=0..n} k! * n^k.
 * https://oeis.org/A368561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k! * n^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    seq_t fact   = 1;          /* 0! */
    seq_t pow    = 1;          /* n^0 */
    seq_t result = 1;          /* 0! * n^0 */

    for (idx_t k = 1; k <= n; ++k) {
        fact *= (seq_t)k;      /* compute k! */
        pow  *= (seq_t)n;      /* compute n^k */
        result += fact * pow;  /* add k! * n^k */
    }
    return result;
}
