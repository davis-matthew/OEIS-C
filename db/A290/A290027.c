/*
 * A290027  Number of 4-cycles in the n-halved cube graph.
 * https://oeis.org/A290027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-5)*C(n,3)*(13*n-35)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* initial values: a(1)=0, a(2)=0, a(3)=3, a(4)=102, a(5)=900 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static const seq_t init[6] = {0, 0, 0, 3, 102, 900};
    if (n > 0 && n < 6) return init[n];

    /* factor 3 */
    seq_t res = (seq_t)3;
    /* factor 2^(n-5) */
    res *= pow2(n - 5);
    /* factor C(n,3) */
    res *= binomial(n, (idx_t)3);
    /* factor (13*n - 35) */
    res *= (seq_t)(13 * n - 35);
    return res;
}
