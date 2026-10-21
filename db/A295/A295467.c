/*
 * A295467  a(n) = (15*n)!*n!/((8*n)!*(5*n)!*(3*n)!).
 * https://oeis.org/A295467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(15*n,7*n)*binomial(7*n,2*n)/binomial(3*n,n)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t tn  = 15 * n;
    idx_t n7  = 7  * n;
    idx_t n2  = 2  * n;
    idx_t n3  = 3  * n;

    seq_t res = binomial(tn, n7);
    res = res * binomial(n7, n2);
    res = res / binomial(n3, n);

    return res;
}
