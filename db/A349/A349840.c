/*
 * A349840  The number of compositions of n using elements from the set
 *          {1,3,5,7,8}.
 * https://oeis.org/A349840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-3)+a(n-5)+a(n-7)+a(n-8), with a(0)=1, a(k)=0
 *            for k<0
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Circular buffer of length 9 that holds the last nine values
       needed for the recurrence.  Element 0 of the buffer holds a(0).  */
    seq_t buf[9] = {0};
    buf[0] = (seq_t)1;   /* a(0) = 1 */

    if (n == 0)
        return buf[0];

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i >= 1) sum += buf[(size_t)((i - 1) % (idx_t)9)];
        if (i >= 3) sum += buf[(size_t)((i - 3) % (idx_t)9)];
        if (i >= 5) sum += buf[(size_t)((i - 5) % (idx_t)9)];
        if (i >= 7) sum += buf[(size_t)((i - 7) % (idx_t)9)];
        if (i >= 8) sum += buf[(size_t)((i - 8) % (idx_t)9)];
        buf[(size_t)(i % (idx_t)9)] = sum;
    }

    return buf[(size_t)(n % (idx_t)9)];
}
