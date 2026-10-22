/*
 * A340452  E.g.f.: Sum_{n>=0} x^n * sinh(2^n*x) / n!.
 * https://oeis.org/A340452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1} binomial(n,k) * 2^{k(n-k)}, where the sum
 *            runs over k with (k ≡ n-1 (mod 2))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* sequence is defined for n >= 1 only */

    seq_t result = 0;
    for (idx_t k = 0; k < n; ++k) {
        /* include k only if k and n-1 have the same parity */
        if ( ((n - 1 - k) & 1ULL) != 0 )
            continue;

        seq_t comb = binomial(n, k);          /* C(n,k) */
        idx_t exp = k * (n - k);              /* exponent of 2 */
        seq_t term = comb * pow2(exp);        /* C(n,k) * 2^{k(n-k)} */
        result += term;                       /* accumulate */
    }
    return result;
}
