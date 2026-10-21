/*
 * A275611  a(0) = 1, a(n) = n * a(n-1)^2 + a(n-1).
 * https://oeis.org/A275611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = n*a(n-1)^2 + a(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t prev = (seq_t)1;           /* a(0) */
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t idx = (seq_t)i;        /* cast index to seq_t */
        prev = idx * prev * prev + prev; /* recurrence */
    }
    return prev;
}
