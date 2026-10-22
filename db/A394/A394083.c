/*
 * A394083  Genus of the n-cycle complement graph.
 * https://oeis.org/A394083
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more, changed
 * Formula:   a(n) = ceil((n^2 - 9*n + 12)/12), except a(7)=1 and a(10)=3
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* offset 3 */
    if (n == 7) return (seq_t)1;
    if (n == 10) return (seq_t)3;

    int64_t num = (int64_t)n * (int64_t)n - 9LL * (int64_t)n + 12LL;
    int64_t res = (num + 11) / 12; /* ceil division by 12, works for negative numerators as well */
    return (seq_t)res;
}
