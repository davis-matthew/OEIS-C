/*
 * A384818  Denominator of the sum of the reciprocals of all square divisors
 *          of all positive integers <= n.
 * https://oeis.org/A384818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Σ_{k=1..⌊√n⌋} ⌊n/k^2⌋ / k^2
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Integer square root of an unsigned 64-bit integer. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = 65536;
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if (mid != 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* undefined for n=0, but safe */
    idx_t limit = isqrt_u64(n);

    seq_t num = 0;                     /* current numerator */
    seq_t den = 1;                     /* current denominator */

    for (idx_t k = 1; k <= limit; ++k) {
        idx_t k2 = k * k;              /* k^2 fits in idx_t for our bounds */
        idx_t m  = n / k2;             /* floor(n/k^2) */
        seq_t t   = (seq_t)k2;         /* denominator of the new term */

        seq_t g = gcd(den, t);         /* common factor between current denom and new denom */

        seq_t mul1 = t / g;            /* factor for new denominator part */
        seq_t mul2 = den / g;          /* factor for new numerator part */

        seq_t new_num = num * mul1 + (seq_t)m * mul2;
        seq_t new_den = den * mul1;

        seq_t h = gcd(new_num, new_den); /* reduce the fraction */
        num = new_num / h;
        den = new_den / h;
    }

    return den;                         /* a(n) is the reduced denominator */
}
