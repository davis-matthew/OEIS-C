/*
 * A247302  Rectangular array read upwards by columns: T = T(n,k) = number of
 *          paths from (0,1) to (n,k), where 0 >= k <= 2, consisting of
 *          segments given by the vectors (1,1), (2,1), (1,-1).
 * https://oeis.org/A247302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 2*a(n-6) + 2*a(n-9) for n > 8, with initial values:
 *            a(0)=0, a(1)=1, a(2)=0, a(3)=1, a(4)=0, a(5)=1, a(6)=0, a(7)=2,
 *            a(8)=1
 * Range:     u8 n <= 36, u16 n <= 63, u32 n <= 123, u64 n <= 240,
 *            u128 n <= 474, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values up to n==8 */
    static const seq_t init[9] = {0,1,0,1,0,1,0,2,1};
    if (n <= 8)
        return init[n];

    /* allocate array of size n+1 */
    seq_t *buf = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!buf)
        abort();

    for (idx_t i = 0; i <= 8; ++i)
        buf[i] = init[i];

    for (idx_t i = 9; i <= n; ++i) {
        /* a(i) = 2*a(i-6) + 2*a(i-9) */
        buf[i] = buf[i-6] + buf[i-6] + buf[i-9] + buf[i-9];
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
