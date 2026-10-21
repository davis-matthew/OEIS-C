/*
 * A245874  Number of length 5+2 0..n arrays with some pair in every
 *          consecutive three terms totalling exactly n.
 * https://oeis.org/A245874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 12*n + 26*n^2 + 39*n^3 + 7*n^4, if n even * a(n) = -9
 *            - 18*n + 23*n^2 + 39*n^3 + 7*n^4, if n odd
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 156, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A245874.
 * The sequence counts 7‑tuples of numbers in {0..n} such that for every
 * consecutive triple some pair sums to n.  For all n ≥ 1 the closed
 * form given above is valid. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cast n to seq_t to perform all arithmetic in that type. */
    seq_t ns = (seq_t)n;           /* n^1 */
    seq_t n2 = ns * ns;            /* n^2 */
    seq_t n3 = n2 * ns;            /* n^3 */
    seq_t n4 = n3 * ns;            /* n^4 */

    if ((n & 1ULL) == 0) {         /* n is even */
        seq_t res = (seq_t)1;
        res += (seq_t)12 * ns;
        res += (seq_t)26 * n2;
        res += (seq_t)39 * n3;
        res += (seq_t)7  * n4;
        return res;
    } else {                       /* n is odd */
        seq_t res = (seq_t)7   * n4;
        res += (seq_t)39  * n3;
        res += (seq_t)23  * n2;
        res -= (seq_t)18  * ns;
        res -= (seq_t)9;
        return res;
    }
}
