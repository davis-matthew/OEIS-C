/*
 * A341036  a(n) = Sum_{k=1..n} gcd(k,n)^(k-2).
 * https://oeis.org/A341036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} gcd(k,n)^(k-2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t term;
        if (k <= 2) {
            /* exponent ≤ 0 ⇒ 1 */
            term = 1;
        } else {
            seq_t g   = gcd((seq_t)k, (seq_t)n);
            seq_t exp = (seq_t)(k - 2);
            term = big_pow(g, exp);
        }
        sum += term;
    }
    return sum;
}
