/*
 * A370903  Partial alternating sums of the powerful part function (A057521).
 * https://oeis.org/A370903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=1..n} (-1)^{k+1} pp(k)
 * Range:     i8 n <= 63, i16 n <= 2115, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   compute_pp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where pp(k) is the largest powerful divisor of k (product of the primes
   dividing k raised to their exponents if the exponent is ≥2). */
/* The sequence is the partial alternating sum of pp(k). */

#include <stdint.h>
#include "../utils/compute_pp.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t pk = compute_pp(k);

        if (k & 1)   /* odd k → positive sign */
            sum += pk;
        else         /* even k → negative sign */
            sum -= pk;
    }

    return sum;
}
