/*
 * A322082  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Sum_{d|n, n/d odd} d^k.
 * https://oeis.org/A322082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the antidiagonal reading of the * square
 *            array A(n,k) = Σ_{d|n, n/d odd} d^k. The indices (row, column) *
 *            for the n-th entry are derived from the antidiagonal order
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 175, u64 n <= 441,
 *            u128 n <= 1139, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Find the diagonal number s such that T(s‑1) < n ≤ T(s) ,
     * where T(s) = s*(s+1)/2.  The loop is inexpensive for the
     * problem size (n ≤ 10 000). */
    idx_t s = 1;
    while ((s * (s + 1ULL)) / 2ULL < n) ++s;

    /* prevT = T(s-1) = (s-1)*s/2 */
    idx_t prevT = (s * (s - 1ULL)) / 2ULL;

    /* Position r within the diagonal (0 … s-1). */
    idx_t r = n - prevT - 1ULL;

    /* Map (s,r) to the row (m) and column (k) of the array:
     *   row   = i = r + 1
     *   column= j = s - 1 - r  */
    idx_t m = r + 1ULL;
    idx_t k = s - 1ULL - r;

    /* Decompose m into 2^e * m0 where m0 is odd.  All divisors that
     * satisfy m/d odd must contain the full 2‑power, so only odd
     * divisors of m0 are relevant. */
    idx_t e = 0;
    idx_t m0 = m;
    while ((m0 & 1ULL) == 0) {
        ++e;
        m0 >>= 1ULL;
    }

    /* Sum over odd divisors d0 of m0:  d0^k . Then multiply by (2^e)^k. */
    seq_t sum = 0;
    for (idx_t d0 = 1; d0 * d0 <= m0; ++d0) {
        if (m0 % d0 == 0) {
            sum += big_pow((seq_t)d0, (seq_t)k);
            idx_t d1 = m0 / d0;
            if (d1 != d0) {
                sum += big_pow((seq_t)d1, (seq_t)k);
            }
        }
    }
    seq_t scale = big_pow((seq_t)2, (seq_t)(e * k));
    return sum * scale;
}
