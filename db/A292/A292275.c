/*
 * A292275  A sequence of rounded numbers useful for entering values over
 *          several orders of magnitude in computer-human interfaces, with 10
 *          values per order of magnitude.
 * https://oeis.org/A292275
 *
 * Offset:    20    (a(20) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^floor(n/10) * f(n mod 10) * where f(0)=1 , f(1)=5/4 ,
 *            f(2)=3/2 , f(3)=2 , * f(4)=5/2 , f(5)=3 , f(6)=4 , f(7)=5 ,
 *            f(8)=6 , f(9)=8
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Undefined for indices below the OEIS offset. Return 0 for convenience. */
    if (n < 20)
        return (seq_t)0;

    idx_t m = n / 10;          /* exponent of 10 */
    idx_t r = n % 10;          /* residue modulo 10 */

    /* Multipliers numerator and denominator for each residue class. */
    static const unsigned num[10] = {
        1,   /* 0: 1     */
        5,   /* 1: 5/4   */
        3,   /* 2: 3/2   */
        2,   /* 3: 2     */
        5,   /* 4: 5/2   */
        3,   /* 5: 3     */
        4,   /* 6: 4     */
        5,   /* 7: 5     */
        6,   /* 8: 6     */
        8    /* 9: 8     */
    };
    static const unsigned den[10] = {
        1,  /* 0 */
        4,  /* 1 */
        2,  /* 2 */
        1,  /* 3 */
        2,  /* 4 */
        1,  /* 5 */
        1,  /* 6 */
        1,  /* 7 */
        1,  /* 8 */
        1   /* 9 */
    };

    /* Compute 10^m */
    seq_t val = pow10(m);

    /* Apply the fractional multiplier */
    val = mul_small(val, num[r]);

    if (den[r] != 1)
        val = val / (seq_t)den[r];

    return val;
}
