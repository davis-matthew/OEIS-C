/*
 * A363974  Expansion of Sum_{k>0} x^k / (1 - x^(2*k))^3.
 * https://oeis.org/A363974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n, d odd} binomial((d+3)/2, 2)
 * Range:     u8 n <= 44, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* that is: for every odd divisor d of n compute
   k = (d+3)/2,  term = k*(k-1)/2, and add all such terms. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0, safety */
        return 0;

    seq_t total = 0;

    /* enumerate divisors d up to sqrt(n) */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            if ((d & 1ULL)) { /* odd? */
                seq_t k = ((seq_t)d + 3) / 2;
                total += k * (k - 1) / 2;
            }

            /* second divisor e = n/d, avoid double counting */
            idx_t e = n / d;
            if (e != d && (e & 1ULL)) {
                seq_t k = ((seq_t)e + 3) / 2;
                total += k * (k - 1) / 2;
            }
        }
    }
    return total;
}
