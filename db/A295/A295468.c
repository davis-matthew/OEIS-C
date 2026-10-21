/*
 * A295468  a(n) =
 *          (30*n)!*(5*n)!*(3*n)!*(2*n)!/((15*n)!*(10*n)!*(8*n)!*(6*n)!*n!).
 * https://oeis.org/A295468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(30*n,15*n)*binomial(15*n,5*n)*binomial(2*n,n) *
 *            / (binomial(8*n,3*n)*binomial(6*n,n))
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t r = binomial(30 * n, 15 * n);   /* C(30n,15n)   */
    r *= binomial(15 * n, 5 * n);         /* *C(15n,5n)   */
    r *= binomial(2 * n, n);             /* *C(2n,n)     */
    r /= binomial(8 * n, 3 * n);          /* /C(8n,3n)    */
    r /= binomial(6 * n, n);             /* /C(6n,n)     */
    return r;
}
