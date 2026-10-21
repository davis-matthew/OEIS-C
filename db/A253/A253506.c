/*
 * A253506  Number of (n+2) X (4+2) 0..1 arrays with every 2 X 2 and 3 X 3
 *          subblock diagonal maximum minus antidiagonal minimum nondecreasing
 *          horizontally and vertically.
 * https://oeis.org/A253506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n) = (n^4 + 18*n^3 + 329*n^2 + 864*n - 36) / 3 for n >
 *            6 a(1)=636, a(2)=1196, a(3)=2088, a(4)=3400, a(5)=5136,
 *            a(6)=7396
 * Range:     u8 none, u16 n <= 9, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const seq_t first6[6] = {
        (seq_t)636,
        (seq_t)1196,
        (seq_t)2088,
        (seq_t)3400,
        (seq_t)5136,
        (seq_t)7396
    };

    if (n <= 6) {
        return first6[n - 1u];
    }

    seq_t nn = (seq_t)n;
    /* compute n^2, n^3, n^4 using seq_t arithmetic (no overflow for n ≤ 10000) */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    /* numerator = n^4 + 18*n^3 + 329*n^2 + 864*n - 36 */
    seq_t num = n4;
    num += 18 * n3;
    num += 329 * n2;
    num += 864 * nn;
    /* subtract 36 */
    num -= 36;

    /* divide by 3 */
    return num / 3;
}
