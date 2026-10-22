/*
 * A330443  Least m >= 0 such that (n+m)(n+m+1)/2 - n(n-3)/2 is prime, or -1
 *          if no such m exists.
 * https://oeis.org/A330443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least m ≥ 0 such that (n+m)(n+m+1)/2 - n(n-3)/2 is prime,
 *            or -1 if no such m exists
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    const idx_t MAX_ITER = 200000;          /* upper bound for searching          */
    seq_t n_val  = (seq_t)n;                /* convert index to seq_t for safety*/
    seq_t base   = n_val * (n_val - 3) / 2; /* constant part of the expression   */

    for (idx_t m = 0; m < MAX_ITER; ++m) {
        seq_t t  = n_val + (seq_t)m;        /* n + m */
        seq_t val = t * (t + 1);           /* (n+m)(n+m+1) */
        val /= 2;                          /* divide by 2                      */
        val -= base;                       /* subtract n(n-3)/2                */

        if (val > 1 && is_prime_big(val)) {
            return (seq_t)m;               /* found the least m that gives a prime */
        }
    }
    return (seq_t)-1;                       /* no prime found within the bound      */
}
