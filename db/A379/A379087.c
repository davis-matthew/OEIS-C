/*
 * A379087  a(n) = Sum_{k=0..floor(n/3)} binomial(3*n+k-1,k) *
 *          binomial(3*n+k,n-3*k).
 * https://oeis.org/A379087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} binomial(3*n+k-1,k) * binomial(3*n+k,
 *            n-3*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t kmax = n / 3;

    for (idx_t k = 0; k <= kmax; ++k)
    {
        idx_t a_n   = 3 * n + k - 1;   /* 3*n+k-1 */
        idx_t b_n   = 3 * n + k;       /* 3*n+k   */
        idx_t b_k   = n - 3 * k;       /* n-3*k   */

        seq_t v1 = binomial(a_n, k);
        seq_t v2 = binomial(b_n, b_k);

        sum += v1 * v2;
    }

    return sum;
}
