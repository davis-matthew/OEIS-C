/*
 * A388283  Numerator of (3*n - sigma(n)) / n.
 * https://oeis.org/A388283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = (3*n - sigma(n)) / gcd( n, 3*n - sigma(n) )
 * Range:     i8 n <= 66, i16 n <= 16410, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 98 terms through n = 9769 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset undefined, harmless */
    seq_t sn   = (seq_t)n;
    seq_t sig  = sigma_of(n);
    seq_t num  = (seq_t)3 * sn - sig;
    seq_t g    = gcd(num, sn);
    return num / g;
}
