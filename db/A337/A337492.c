/*
 * A337492  a(n) = a(n-1) + 4*a(n-3) + 2*a(n-4) + 2*a(n-5); a(0) = a(1) = a(2)
 *          = 1, a(3) = 5, a(4) = 11.
 * https://oeis.org/A337492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = a(n-1) + 4*a(n-3) + 2*a(n-4) + 2*a(n-5);
 *   a(0)=1, a(1)=1, a(2)=1, a(3)=5, a(4)=11
 */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle trivial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)11;

    /* Circular buffer holding the last 6 values */
    seq_t buf[6] = { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)11, (seq_t)0 };

    /* Compute terms 5 .. n */
    for (idx_t i = 5; i <= n; ++i) {
        int idx = (int)(i % 6);
        int idx_m1 = (int)((i + 5) % 6);      /* (i-1) mod 6 */
        int idx_m3 = (int)((i + 3) % 6);      /* (i-3) mod 6 */
        int idx_m4 = (int)((i + 2) % 6);      /* (i-4) mod 6 */
        int idx_m5 = (int)((i + 1) % 6);      /* (i-5) mod 6 */

        buf[idx] = buf[idx_m1] + (seq_t)4 * buf[idx_m3]
                     + (seq_t)2 * buf[idx_m4]
                     + (seq_t)2 * buf[idx_m5];
    }

    return buf[(int)(n % 6)];
}
