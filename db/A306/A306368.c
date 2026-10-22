/*
 * A306368  a(n) = numerator of (n + 3)*(n + 4)/((n + 1)*(n + 2)).
 * https://oeis.org/A306368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of ((n + 3)*(n + 4))/((n + 1)*(n + 2))
 * Range:     u8 n <= 12, u16 n <= 358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = (n + 3)*(n + 4) / gcd( (n + 3)*(n + 4), (n + 1)*(n + 2) ) */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Cast n to seq_t before arithmetic to avoid overflow in idx_t */
    seq_t m = (seq_t)n;

    seq_t num = (m + 3) * (m + 4);   /* numerator = (n+3)*(n+4) */
    seq_t den = (m + 1) * (m + 2);   /* denominator = (n+1)*(n+2) */

    seq_t g = gcd(num, den);         /* greatest common divisor */

    return num / g;                  /* reduced numerator */
}
