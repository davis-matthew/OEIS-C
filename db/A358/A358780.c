/*
 * A358780  Dirichlet g.f.: zeta(s) * zeta(2*s) * zeta(3*s) * zeta(4*s).
 * https://oeis.org/A358780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   part_le4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Multiplicative sequence with a(p^e) = number of partitions of e into parts
 * 1, 2, 3, 4.  Equivalently, a(n) has Dirichlet generating function
 * ζ(s)ζ(2s)ζ(3s)ζ(4s).
 */
#include <stdint.h> /* for uint64_t, if necessary */
#include "../utils/part_le4.h"

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out-of-range, safety */
    idx_t temp = (idx_t)n;
    seq_t res = 1;

    /* handle factor 2 */
    int exp = 0;
    while ((temp & 1) == 0) {        /* temp % 2 == 0 */
        temp >>= 1;
        ++exp;
    }
    if (exp > 0) res *= part_le4(exp);

    /* handle odd factors */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            exp = 0;
            while (temp % d == 0) {
                temp /= d;
                ++exp;
            }
            res *= part_le4(exp);
        }
    }

    /* if a prime factor remains (exponent 1) it contributes 1 */
    return res;
}
