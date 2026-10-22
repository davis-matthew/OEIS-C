/*
 * A323171  Numerator of the average of distinct prime factors of n
 *          (A008472(n)/A001221(n)).
 * https://oeis.org/A323171
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of the reduced fraction A008472(n)/A001221(n) *
 *            (sum of distinct prime factors of n divided by their count). *
 *            If the fraction is a/k (in lowest terms), the function returns a
 * Range:     u8 n <= 256, u16 n <= 9670, u32 n <= 9670, u64 n <= 9670,
 *            u128 n <= 9670, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2)          /* the sequence is defined only for n ≥ 2 */
        return 0;

    /* Local working copies suitable for arbitrary sized seq_t. */
    seq_t sum   = 0;    /* sum of distinct prime factors              */
    seq_t count = 0;    /* number of distinct prime factors          */

    /* Use a uint64_t for iteration; conversion to seq_t happens when
       adding to sum / count.                           */
    uint64_t tmp   = (uint64_t)n;
    uint64_t limit = tmp;

    /* Trial division up to sqrt(tmp).  Use i <= limit / i to avoid
       overflow of i*i.                                         */
    for (uint64_t i = 2; i <= limit / i; ++i) {
        if (tmp % i == 0) {
            sum   += (seq_t)i;
            count += (seq_t)1;
            while (tmp % i == 0)
                tmp /= i;
            /* After the division the current prime is removed from
               tmp, so the loop condition is still valid.         */
        }
    }

    /* If a prime larger than sqrt(n) remains, it is a single factor. */
    if (tmp > 1) {
        sum   += (seq_t)tmp;
        count += (seq_t)1;
    }

    /* Reduce the fraction sum/count to lowest terms.  Since count>0
       for n≥2, gcd is defined.                                     */
    seq_t g = gcd(sum, count);
    return sum / g;
}
