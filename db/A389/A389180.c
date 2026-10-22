/*
 * A389180  Number of mutual-visibility sets in the n-helm graph.
 * https://oeis.org/A389180
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 15*n + 1 for n >= 7; a(3)=57, a(4)=159, a(5)=380,
 *            a(6)=836
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n >= 7) {
        seq_t val = pow3(n);                /* 3^n */
        val += (seq_t)15 * (seq_t)n;        /* + 15*n */
        val += (seq_t)1;                   /* + 1 */
        return val;
    }
    /* special cases for 3 <= n < 7 */
    if (n == 3) return (seq_t)57;
    if (n == 4) return (seq_t)159;
    if (n == 5) return (seq_t)380;
    if (n == 6) return (seq_t)836;
    /* for n < 3 the OEIS offset does not define values; return 0 */
    return (seq_t)0;
}
