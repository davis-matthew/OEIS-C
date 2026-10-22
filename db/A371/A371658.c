/*
 * A371658  G.f. satisfies A(x) = 1 + x * A(x)^2 * (1 + A(x))^2.
 * https://oeis.org/A371658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * sum_{k=0..floor((n-1)/2)} 4^(n-k) * C(n,k) *
 *            C(3*n-k, n-1-2*k), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* a(0) = 1 */

    seq_t sum = 0;
    idx_t limit = (n - 1) / 2;      /* floor((n-1)/2) */

    for (idx_t k = 0; k <= limit; ++k)
    {
        /* term = 4^(n-k) * C(n,k) * C(3*n - k, n-1-2*k) */
        seq_t term = pow_uint(4, n - k);
        term = term * binomial(n, k);
        term = term * binomial(3 * n - k, n - 1 - 2 * k);
        sum += term;
    }

    /* divide by n (n > 0 here) */
    return sum / (seq_t)n;
}
