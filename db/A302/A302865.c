/*
 * A302865  a(n) = (4*n+2)*a(n-1) + (-1)^(n+1)*4*((n-1)!), with a(0) = 8, n >
 *          0.
 * https://oeis.org/A302865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n + 2)*a(n-1) + (-1)^(n+1)*4*((n-1)!) with a(0) = 8
 *            Recurrence implemented iteratively
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 8;

    seq_t a_prev = 8;            /* a(0) */
    seq_t fact   = 1;            /* (n-1)! for n = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term1 = (seq_t)(4 * i + 2) * a_prev;
        seq_t term2 = (seq_t)4 * fact;

        /* (-1)^(i+1): positive when i is odd, negative when i is even */
        if (i & 1)          /* i odd  => +1 */
            a_prev = term1 + term2;
        else                /* i even => -1 */
            a_prev = term1 - term2;

        /* Update factorial for next iteration: fact = i!  */
        fact *= i;
    }
    return a_prev;
}
