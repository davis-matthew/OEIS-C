/*
 * A397024  a(n) is the sum of gcd(k,n) over all numbers k obtained by
 *          changing any subset of the 1-bits of n to 0-bits in its binary
 *          representation.
 * https://oeis.org/A397024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k AND n = k} gcd(k,n)
 * Range:     u8 n <= 62, u16 n <= 8126, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* a(0) = 0 by definition */

    seq_t sum = 0;
    seq_t nseq = n;
    idx_t sub = n;

    /* iterate over all submasks of n, including 0 */
    while (1) {
        sum += gcd((seq_t)sub, nseq);
        if (sub == 0)
            break;
        sub = (sub - 1) & n;
    }
    return sum;
}
