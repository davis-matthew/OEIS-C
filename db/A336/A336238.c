/*
 * A336238  a(1) = 3; if n>1, and gcd(a(n-1), n) > 1 then a(n) =
 *          a(n-1)/gcd(a(n-1), n), otherwise a(n) = a(n-1) + n.
 * https://oeis.org/A336238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3; for n>1, a(n) = a(n-1)/gcd(a(n-1),n) if gcd>1,
 *            otherwise a(n) = a(n-1)+n
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, safe default */
    if (n == 1) return (seq_t)3;          /* initial value */

    seq_t prev = (seq_t)3;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t g = gcd(prev, (seq_t)i);
        if (g > (seq_t)1) {
            prev = prev / g;
        } else {
            prev = prev + (seq_t)i;
        }
    }
    return prev;
}
