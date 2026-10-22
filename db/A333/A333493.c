/*
 * A333493  a(n) = Sum_{k=1..n} (-1)^(k+1) * lcm(n,k) / gcd(n,k).
 * https://oeis.org/A333493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * lcm(n,k)/gcd(n,k) =
 *            Sum_{k=1..n} (-1)^(k+1) * n*k / gcd(n,k)^2
 * Range:     i8 n <= 9, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* graceful handling for n=0 */

    seq_t sum     = 0;
    seq_t sn      = (seq_t)n;        /* cast once for efficiency */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t sk = (seq_t)k;
        seq_t g  = gcd(sn, sk);     /* gcd(n,k)  (static function in gcd.h) */

        /* Term = n*k / g^2  (exact division) */
        seq_t term = (sn * sk) / (g * g);

        /* Add for odd k, subtract for even k */
        if ((k & 1U) == 0) term = -term;
        sum += term;
    }
    return sum;
}
