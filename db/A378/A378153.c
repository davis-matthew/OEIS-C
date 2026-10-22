/*
 * A378153  G.f. A(x) satisfies A(x) = 1 + (x * (1+x))^3 * A(x)^2.
 * https://oeis.org/A378153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(3*k, n-3*k) * Catalan(k)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 33, u64 n <= 61, u128 n <= 116,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t max_k = n / 3;
    seq_t sum = 0;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t t = binomial(3 * k, n - 3 * k);
        seq_t c = catalan(k);
        sum += t * c;
    }
    return sum;
}
