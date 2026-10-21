/*
 * A244158  If n = Sum c_i * 10^i then a(n) = Sum c_i * Cat(i+1), where Cat(k)
 *          = A000108(k).
 * https://oeis.org/A244158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = Σ_{i≥0} c_i * Cat(i+1) where n = Σ c_i * 10^i
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Catalan number helper is provided by the framework. */
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t i = 0;

    while (n != 0)
    {
        idx_t digit = n % 10;        /* current decimal digit */
        seq_t cat   = catalan(i + 1);/* Cat(i+1) */
        res += (seq_t)digit * cat;   /* accumulate contribution */
        n /= 10;                     /* move to next digit */
        ++i;
    }

    return res;                       /* res is 0 when n == 0 */
}
