/*
 * A268358  Number of n-digit numbers in base ten having at least five
 *          different digits with no leading zeros allowed.
 * https://oeis.org/A268358
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*10^(n-1) - 189*4^n + 648*3^n - 1701*2^(n-1) + 504
 *            (valid for all n >= 5; returns 0 for n < 5)
 * Range:     u8 none, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   mul_small, pow10, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    seq_t res;

    /* 9*10^(n-1) */
    res = mul_small(pow10(n - 1), 9);

    /* -189*4^n */
    {
        seq_t tmp = mul_small(pow_uint(4, n), 189);
        res -= tmp;
    }

    /* +648*3^n */
    {
        seq_t tmp = mul_small(pow_uint(3, n), 648);
        res += tmp;
    }

    /* -1701*2^(n-1) */
    {
        seq_t tmp = mul_small(pow_uint(2, n - 1), 1701);
        res -= tmp;
    }

    /* +504 */
    res += 504;

    return res;
}
