/*
 * A399878  a(n) = Sum_{k = 1..n, k does not divide n} (gcd(n, floor(n/k) + 1)
 *          - gcd(n, floor(n/k))).
 * https://oeis.org/A399878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, changed
 * Formula:   a(n) = Sum_{k=1..n, k∤n} (gcd(n, floor(n/k)+1) - gcd(n,
 *            floor(n/k)))
 * Range:     i8 n <= 154, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (n % k == 0)
            continue;               /* skip divisors */
        idx_t t = n / k;               /* floor(n / k) */
        seq_t g1 = gcd((seq_t)n, (seq_t)(t + 1));
        seq_t g0 = gcd((seq_t)n, (seq_t)t);
        res += g1 - g0;
    }
    return res;
}
