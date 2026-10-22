/*
 * A390011  E.g.f. A(x) satisfies A(x) = exp( x * (1-x) * A(x)^2 ).
 * https://oeis.org/A390011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (-1)^(n-k) * (2*k+1)^(k-1) *
 *            binomial(k, n-k) / k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, mul_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return (seq_t)1;

    seq_t pos = 0;          /* sum of terms with positive sign */
    seq_t neg = 0;          /* sum of terms with negative sign */
    seq_t prod = 1;         /* product of integers (k+1)...n for current k */

    for (idx_t k = n; k > 0; --k) {
        /* binomial(k, n-k) is zero for k < n-k, but multiplying by zero
           automatically gives zero, so the test is omitted. */
        seq_t bin   = binomial(k, n - k);
        seq_t pow   = pow_seq((seq_t)(2 * k + 1), (int)(k - 1));
        seq_t term  = bin * pow;
        term        = term * prod;    /* term *= n! / k!   */

        if (((n - k) & 1) == 0)       /* even exponent => positive sign */
            pos = pos + term;
        else                          /* odd exponent => negative sign */
            neg = neg + term;

        /* prepare prod for the next smaller k: prod *= k */
        prod = mul_small(prod, (unsigned)k);
    }

    /* final result = pos - neg; pos >= neg because the sequence is positive */
    return pos - neg;
}
