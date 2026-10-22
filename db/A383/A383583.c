/*
 * A383583  a(n) = Sum_{k=0..floor(n/3)} binomial(n-2*k-1,k) *
 *          binomial(k,n-3*k).
 * https://oeis.org/A383583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} binomial(n-2k-1, k) * binomial(k, n-3k)
 * Range:     u8 n <= 18, u16 n <= 31, u32 n <= 54, u64 n <= 101,
 *            u128 n <= 194, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t maxk = n / 3;               /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin1;
        /* handle n-2k-1 possibly negative */
        if (k == 0) {
            /* binomial(-1,0) is 1 */
            bin1 = 1;
        } else if (n <= 2 * k) {
            bin1 = 0;
        } else {
            bin1 = binomial(n - 2 * k - 1, k);
        }

        seq_t bin2;
        if (n >= 3 * k) {
            idx_t r = n - 3 * k;
            bin2 = binomial(k, r);
        } else {
            bin2 = 0;
        }

        total += bin1 * bin2;
    }
    return total;
}
