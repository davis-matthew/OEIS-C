/*
 * A272128  a(n) = 30240*n^5-25200*n^4+5040*n^3+7320*n^2-2638*n+1.
 * https://oeis.org/A272128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 30240*n^5-25200*n^4+5040*n^3+7320*n^2-2638*n+1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 10, u64 n <= 906, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute using signed 128‑bit arithmetic to avoid overflow in intermediate
       steps; the final result is positive and cast to seq_t. */
    __int128 sn = (__int128)n;   /* promote the index to signed 128‑bit */
    __int128 res = 30240;
    res = res * sn - 25200;
    res = res * sn + 5040;
    res = res * sn + 7320;
    res = res * sn - 2638;
    res = res * sn + 1;
    return (seq_t)res;
}
