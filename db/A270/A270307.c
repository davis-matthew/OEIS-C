/*
 * A270307  Expansion of -(4*x^3+8*x^2+4*x+1)/(2*x^4+4*x^3+2*x^2-x-1).
 * https://oeis.org/A270307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -(a(n-1)) + 2*a(n-2) + 4*a(n-3) + 2*a(n-4) (obtained from
 *            the rational generating function
 *            -(4*x^3+8*x^2+4*x+1)/(2*x^4+4*x^3+2*x^2-x-1) ) Initial values:
 *            a(0)=1, a(1)=3, a(2)=7, a(3)=7
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 76, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)7;

    /* Sliding window for the last four values:
       v0 = a(i-4), v1 = a(i-3), v2 = a(i-2), v3 = a(i-1) */
    seq_t v0 = (seq_t)1;  /* a(0) */
    seq_t v1 = (seq_t)3;  /* a(1) */
    seq_t v2 = (seq_t)7;  /* a(2) */
    seq_t v3 = (seq_t)7;  /* a(3) */
    seq_t val;

    for (idx_t i = 4; i <= n; ++i) {
        /* val = -v3 + 2*v2 + 4*v1 + 2*v0   */
        val = -v3;
        val = val + v2;  /* first + */
        val = val + v2;  /* second + for the 2*v2 term */
        val = val + v1;  /* first + for the 4*v1 term */
        val = val + v1;  /* second + for the 4*v1 */
        val = val + v1;  /* third + for the 4*v1 */
        val = val + v1;  /* fourth + for the 4*v1 */
        val = val + v0;  /* first + for the 2*v0 term */
        val = val + v0;  /* second + for the 2*v0 */

        /* Shift the window for next iteration */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = val;
    }

    return v3;  /* after loop v3 holds a(n) */
}
