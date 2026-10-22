/*
 * A334580  Denominator of Sum_{k=1..n} (-1)^(k+1)/k^2.
 * https://oeis.org/A334580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of S_n where S_n = Σ_{k=1..n} (-1)^(k+1)/k^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static void add_signed(seq_t *mag, int *sign,
                       seq_t mag1, int sign1,
                       seq_t mag2, int sign2)
{
    if (sign1 == 0) { *mag = mag2; *sign = sign2; return; }
    if (sign2 == 0) { *mag = mag1; *sign = sign1; return; }

    if (sign1 == sign2) {
        *mag = mag1 + mag2;
        *sign = sign1;
    } else {
        if (mag1 > mag2) {
            *mag = mag1 - mag2;
            *sign = sign1;
        } else if (mag2 > mag1) {
            *mag = mag2 - mag1;
            *sign = sign2;
        } else {
            *mag = 0;
            *sign = 0;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1, return 0 */

    seq_t num_mag = 0;   /* absolute value of numerator   */
    int   num_sign = 0;  /* +1, -1, or 0                */
    seq_t den = 1;       /* denominator, always > 0     */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k * (seq_t)k;        /* k^2 */
        seq_t r = num_mag * kk;               /* old_num * k^2 */

        /* sign of the k-th term of the series */
        int term_sign = (k & 1) ? +1 : -1;     /* +1 for odd k, -1 for even k */

        /* new numerator (signed) */
        seq_t new_mag;
        int   new_sign;
        add_signed(&new_mag, &new_sign, r, num_sign, den, term_sign);

        /* new denominator (positive) */
        seq_t new_den = den * kk;

        /* Reduce fraction by gcd of |numerator| and denominator */
        if (new_mag != 0) {
            seq_t g = gcd(new_mag, new_den);
            new_mag = new_mag / g;
            new_den = new_den / g;
        } else {
            new_den = 1;
        }

        /* update for next iteration */
        num_mag = new_mag;
        num_sign = new_sign;
        den = new_den;
    }

    return den;
}
