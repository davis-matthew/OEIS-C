/*
 * A389872  G.f. A(x) satisfies A(x) = 1 + x*(1+x^2)^3*A(x)^2.
 * https://oeis.org/A389872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(3*(n-2*k),k) *
 *            Catalan(n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n >> 1;           /* floor(n/2) */
    for (idx_t k = 0; k <= kmax; ++k)
    {
        idx_t m = n - 2*k;         /* n - 2*k */
        seq_t bin = binomial(3 * m, k);   /* binomial(3*(n-2*k), k) */
        seq_t cat = catalan(m);            /* Catalan(n-2*k) */
        sum += bin * cat;                     /* accumulation */
    }
    return sum;
}
