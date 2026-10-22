/*
 * A398182  Degree of the minimal polynomial whose root is the sidelength of
 *          the largest n-cube that fits inside a unit (n+1)-cube.
 * https://oeis.org/A398182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=2, a(2)=2; For n>2 even: a(n)=2^(n-2)*(2^(n‑1)-1);
 *            For n>1 odd: a(n)=2^(2*n-3)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return 0;
    if (n == 1ULL || n == 2ULL)
        return 2;
    if ((n & 1ULL) == 0ULL) {              /* n even > 2 */
        seq_t two_to_n_1 = pow2(n - 1);
        return pow2(n - 2) * (two_to_n_1 - 1);
    } else {                               /* n odd > 1 */
        return pow2(2 * n - 3);
    }
}
