/*
 * A364813  a(n) = Product_{k=2..n} k^ord(n, k) where ord(n, k) = 0 if k does
 *          not divide n, otherwise ord(n, k) = e where e is such that k^e
 *          divides n but k^(e + 1) does not.
 * https://oeis.org/A364813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=2..n} k^ord(n,k) where ord(n,k) = 0 if k∤n,
 *            otherwise the largest e with k^e | n
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 47, u64 n <= 143, u128 n <= 719,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;

    seq_t result = 1;
    for (idx_t k = 2; k <= n; ++k) {
        if (n % k != 0)
            continue;
        idx_t t = n;
        int exp = 0;
        while (t % k == 0) {
            ++exp;
            t /= k;
        }
        result = result * pow_uint(k, exp);
    }
    return result;
}
