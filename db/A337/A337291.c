/*
 * A337291  a(n) = 3*binomial(4*n,n)/(4*n-1).
 * https://oeis.org/A337291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(4*n,n)/(4*n-1)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* D‑finite recurrence: 3*n*(3*n-1)*(3*n-2)*a(n) =
   8*(4*n-5)*(4*n-3)*(2*n-1)*a(n-1),   a(1)=4. */
/* Uses exact integer arithmetic with gcd‑cancellation to keep
   intermediate values small. */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* undefined for n < 1, offset 1 */

    seq_t a_val = 4;              /* a(1) */
    for (idx_t k = 2; k <= n; ++k) {
        /* numerator and denominator of multiplier */
        seq_t num = (seq_t)8 *
                    (seq_t)(4*k-5) *
                    (seq_t)(4*k-3) *
                    (seq_t)(2*k-1);

        seq_t den = (seq_t)3 *
                    (seq_t)k *
                    (seq_t)(3*k-1) *
                    (seq_t)(3*k-2);

        /* cancel common factor between num and den */
        seq_t g = gcd(num, den);
        num /= g;  den /= g;

        /* cancel common factor between a_val and den */
        g = gcd(a_val, den);
        a_val /= g;  den /= g;   /* den divides num * a_val exactly now */

        /* update a_val */
        a_val = (a_val * num) / den;
    }
    return a_val;
}
