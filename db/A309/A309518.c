/*
 * A309518  Number of even parts in the partitions of n into 4 parts.
 * https://oeis.org/A309518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 4*a(n-4) + 4*a(n-5) -
 *            3*a(n-6) + a(n-7) + a(n-8) - 3*a(n-9) + 4*a(n-10) - 4*a(n-11) +
 *            4*a(n-12) - 4*a(n-13) + 3*a(n-14) - a(n-15) for n ≥ 15, with the
 *            initial values
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=1,a(6)=2,a(7)=5,a(8)=8,
 *            a(9)=10,a(10)=14,a(11)=19,a...
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the number of even parts in the partitions of n into four parts. */
seq_t a(idx_t n)
{
    /* precomputed values for n = 0..14 */
    static const seq_t init[15] = {
        0, 0, 0, 0, 0, 1, 2, 5, 8, 10, 14, 19, 24, 32, 40
    };

    /* For small n we simply return the precomputed value. */
    if (n < 15)
        return init[n];

    /* Use a circular buffer of the last 15 computed values. */
    long long buf[15];
    for (idx_t i = 0; i < 15; ++i)
        buf[i] = (long long)init[i];

    for (idx_t i = 15; i <= n; ++i) {
        long long t =
            3LL * buf[(i - 1) % 15]   - 4LL * buf[(i - 2) % 15]
          + 4LL * buf[(i - 3) % 15]   - 4LL * buf[(i - 4) % 15]
          + 4LL * buf[(i - 5) % 15]   - 3LL * buf[(i - 6) % 15]
          +     buf[(i - 7) % 15]     +     buf[(i - 8) % 15]
          - 3LL * buf[(i - 9) % 15]   + 4LL  * buf[(i - 10) % 15]
          - 4LL * buf[(i - 11) % 15]  + 4LL  * buf[(i - 12) % 15]
          - 4LL * buf[(i - 13) % 15]  + 3LL  * buf[(i - 14) % 15]
          -     buf[(i - 15) % 15];

        buf[i % 15] = t;
    }

    return (seq_t)buf[n % 15];
}
