/*
 * A366554  G.f. A(x) satisfies A(x) = 1 + x + x^4*A(x)^2.
 * https://oeis.org/A366554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} binomial(k+1, n-4k) * Catalan(k)
 * Range:     u8 n <= 20, u16 n <= 34, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t max_k = n / 4;          /* largest k with 4*k <= n */
    seq_t sum = 0;                /* accumulator for the result */

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t offset = n - 4 * k;             /* n - 4k : guaranteed >= 0 */
        if (offset > k + 1)                   /* binomial(k+1,offset) would be zero */
            continue;

        seq_t bin   = binomial(k + 1, offset);/* C(k+1, n-4k)          */
        seq_t cat   = catalan(k);             /* Catalan(k)             */
        sum += bin * cat;                     /* accumulate term         */
    }

    return sum;
}
