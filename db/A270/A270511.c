/*
 * A270511  Number of 3X3X3 triangular 0..n arrays with some element plus some
 *          adjacent element totalling n+1 exactly once.
 * https://oeis.org/A270511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { 9*n^5 - 36*n^4 + 96*n^3 - 111*n^2 + 54*n for n even
 *            9*n^5 - 36*n^4 + 108*n^3 - 156*n^2 + 132*n - 42 for n odd }
 *            (OEIS A270511, offset 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 55, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safe guard, offset starts at 1 */

    seq_t s  = (seq_t)n;
    seq_t s2 = s * s;          /* n^2 */
    seq_t s3 = s2 * s;         /* n^3 */
    seq_t s4 = s3 * s;         /* n^4 */
    seq_t s5 = s4 * s;         /* n^5 */

    seq_t result;
    if ((n & 1ULL) == 0) {     /* n even */
        result = 9 * s5 - 36 * s4 + 96 * s3 - 111 * s2 + 54 * s;
    } else {                   /* n odd */
        result = 9 * s5 - 36 * s4 + 108 * s3 - 156 * s2 + 132 * s - 42;
    }
    return result;
}
