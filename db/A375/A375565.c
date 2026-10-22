/*
 * A375565  a(n) = Sum_{k=0..floor(n/3)} (n-2*k+1) * binomial(n-2*k,k)^2.
 * https://oeis.org/A375565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} (n-2k+1) * C(n-2k, k)²
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n / 3; ++k) {
        idx_t m = n - 2 * k;               /* m = n - 2k */
        seq_t b = binomial(m, k);          /* C(m, k)      */
        seq_t b_sq = b * b;                /* C(m, k)²     */
        seq_t factor = (seq_t)(m + 1);     /* n-2k+1 == m+1 */
        result += b_sq * factor;           /* add term      */
    }
    return result;
}
