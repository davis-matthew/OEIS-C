/*
 * A324302  a(n) = [x^(n*(n+1))] Sum_{m>=0} x^m * (x^(m+1) + i)^m / (1 +
 *          i*x^(m+1))^(m+1), for n >= 0.
 * https://oeis.org/A324302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^(n*(n+1))] Sum_{m>=0} x^m * (x^(m+1) + i)^m / (1 +
 *            i*x^(m+1))^(m+1), equivalent to a(n) = Sum_{d | (n*(n+1)+1)}
 *            (-1)^{(m+r)/2 + r} * Delannoy(m,r), where d runs over all
 *            divisors of n*(n+1)+1, m = d - 1, r = ( (n*(n+1)+1) / d ) - 1,
 *            Delannoy(m,r) = Σ_{k=0}^{min(m,r)} binom(m,k) * binom(...
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 15, i64 n <= 29, i128 n <= 54,
 *            big any n
 * Helpers:   add_divisor_contrib, delannoy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/add_divisor_contrib.h"
#include "../utils/delannoy.h"

seq_t a(idx_t n)
{
    /* compute a(n) = coefficient of x^{n*(n+1)} in the given generating function */
    seq_t N = (seq_t)n * ((seq_t)n + 1);   /* N = n*(n+1) */
    seq_t E = N + 1;                       /* E = n*(n+1)+1, always odd */

    seq_t result = 0;
    uint64_t E_u64 = (uint64_t)E;          /* fits in 64‑bit for n <= 10000 */
    for (uint64_t d = 1; d * d <= E_u64; d += 2)   /* iterate only odd divisors */
    {
        if (E_u64 % d == 0)
        {
            add_divisor_contrib(E, d, &result);
            uint64_t other = E_u64 / d;
            if (other != d && (other & 1))
                add_divisor_contrib(E, other, &result);
        }
    }
    return result;
}
