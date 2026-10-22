/*
 * A362324  a(n) = n! * Sum_{k=0..floor(n/5)} (-n)^k / (k! * (n-5*k)!).
 * https://oeis.org/A362324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/5⌋} (-n)^k / (k! * (n-5k)!)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 19, i128 n <= 29,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a carefully ordered multiplication and division to avoid overflow
   on machines where seq_t is 64‑bit.  For larger seq_t types the
   operations are exact. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    idx_t m = n / 5;          /* floor(n/5) */
    seq_t sum = 1;            /* term for k = 0 */
    seq_t term = 1;           /* current term (term_{k-1}) */

    for (idx_t k = 1; k <= m; ++k) {
        seq_t denom = (seq_t)k;        /* denominator = k! divided progressively */

        /* multiply by -n */
        term *= -(seq_t)n;
        seq_t g = gcd(term, denom);
        if (g != 1) {
            term /= g;
            denom /= g;
        }

        /* multiply by the five consecutive integers */
        for (idx_t i = 0; i < 5; ++i) {
            seq_t val = (seq_t)(n - 5 * k + 5 - i);
            term *= val;
            g = gcd(term, denom);
            if (g != 1) {
                term /= g;
                denom /= g;
            }
        }

        /* any remaining divisor should be 1, but reduce just in case */
        if (denom != 1) term /= denom;

        sum += term;
    }

    return sum;
}
