/*
 * A263366  Number of (n+1) X (1+1) 0..2 arrays with each row and column
 *          divisible by 7, read as a base-3 number with top and left being
 *          the most significant digits.
 * https://oeis.org/A263366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) with initial terms: a(1)=1, a(2)=1, a(3)=2, a(4)=5,
 *            a(5)=10, a(6)=19
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Initial values for n = 1..6 */
    static const seq_t init[6] = { (seq_t)1, (seq_t)1, (seq_t)2,
                                   (seq_t)5, (seq_t)10, (seq_t)19 };

    if (n == 0) return (seq_t)0;          /* not used (offset 1) */
    if (n <= 6) return init[n-1];

    /* Buffer holding the last six values: buf[0] = a(n-6), ..., buf[5] = a(n-1) */
    seq_t buf[6];
    for (idx_t i = 0; i < 6; ++i) buf[i] = init[i];

    seq_t cur = 0;
    for (idx_t j = 7; j <= n; ++j) {
        /* Compute a(j) from previous six values */
        cur = (seq_t)3 * buf[5] - (seq_t)3 * buf[4] + (seq_t)3 * buf[3]
              - (seq_t)3 * buf[2] + (seq_t)3 * buf[1] - (seq_t)2 * buf[0];

        /* Shift buffer left by one position */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = cur;
    }
    return cur;
}
