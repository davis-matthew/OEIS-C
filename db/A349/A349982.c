/*
 * A349982  a(n) = 199 + (a(n-1)+a(n-2))/gcd(a(n-1),a(n-2)), a(0)=1 and
 *          a(1)=2.
 * https://oeis.org/A349982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, for n≥2: a(n)=199 +
 *            (a(n-1)+a(n-2))/gcd(a(n-1),a(n-2))
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the two initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Use two variables for the previous two terms to avoid dynamic allocation */
    seq_t prev2 = (seq_t)1;  /* a(n-2) */
    seq_t prev1 = (seq_t)2;  /* a(n-1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = prev1 + prev2;                 /* a(n-1)+a(n-2) */
        seq_t g   = gcd(prev1, prev2);             /* gcd(a(n-1),a(n-2)) */
        current    = (seq_t)199 + sum / g;         /* a(n) */
        prev2 = prev1;                             /* shift window */
        prev1 = current;
    }
    return prev1; /* a(n) */
}
