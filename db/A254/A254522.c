/*
 * A254522  Numerators of (2^n - 1 + (-1)^n)/(2*n), n > 0.
 * https://oeis.org/A254522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) / gcd(2^(n-1), n) for even n * a(n) = (2^(n-1)-1)
 *            / gcd(2^(n-1)-1, n) for odd n * (equivalent to the numerator of
 *            (2^n - 1 + (-1)^n)/(2*n))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n-1));   /* 2^(n-1) */

    if ((n & 1) == 0) {               /* even n */
        seq_t g = gcd(pow2, (seq_t)n);    /* gcd(2^(n-1), n) */
        return pow2 / g;
    } else {                           /* odd n */
        seq_t num = pow2 - (seq_t)1;      /* 2^(n-1) - 1 */
        seq_t g = gcd(num, (seq_t)n);     /* gcd(2^(n-1)-1, n) */
        return num / g;
    }
}
