/*
 * A304275  a(n) = Sum_{k = 1..n} gcd(k,n) / cos(Pi*k/n)^2 for odd n.
 * https://oeis.org/A304275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3) * Sum_{k=1..4*n-2} (-1)^k * gcd(k, 4*n-2)^2 For odd
 *            n, this equals Sum_{k=1..n} gcd(k,n)/cos(Pi*k/n)^2
 * Range:     u8 n <= 4, u16 n <= 111, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Handle the trivial case n = 0 (unused) */
    if (n == 0)
        return 0;

    /* m = 4*n - 2   (fits in idx_t for the given limits) */
    idx_t m = n * 4 - 2;

    seq_t sum = 0;
    for (idx_t k = 1; k <= m; ++k) {
        seq_t g = gcd((seq_t)k, (seq_t)m);
        seq_t gsq = g * g;
        if (k & 1)        /* odd k: sign = -1 */
            sum -= gsq;
        else              /* even k: sign = +1 */
            sum += gsq;
    }
    return sum / 3;
}
