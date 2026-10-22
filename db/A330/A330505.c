/*
 * A330505  Expansion of e.g.f. Sum_{k>=1} arctanh(x^k).
 * https://oeis.org/A330505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{d | n, n/d odd} d This is equivalent to
 *            a(n) = (n-1)! * 2^{v_2(n)} * σ(m) where n = 2^k * m (m odd), and
 *            σ(m) is the sum of divisors of m. Computation uses integer
 *            arithmetic only, no floating point
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   factor_u64, factorial, odd_divisor_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/factorial.h"
#include "../utils/odd_divisor_sum.h"

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the caller (offset 1) */
    /* 1. Compute factorial (n-1)! */
    seq_t fact = factorial(n - 1);

    /* 2. Split n into 2^k * m, where m is odd */
    unsigned int k = 0;
    uint64_t m = (uint64_t)n;
    while ((m & 1ULL) == 0ULL)
    {
        ++k;
        m >>= 1;
    }

    /* 3. Compute 2^k as seq_t */
    seq_t two_pow = (seq_t)1;
    while (k--)
        two_pow *= (seq_t)2;

    /* 4. Compute σ(m) (sum of divisors of the odd part) */
    seq_t sigma = odd_divisor_sum(m);

    /* 5. Result = fact * two_pow * sigma */
    seq_t result = fact;
    result *= two_pow;
    result *= sigma;

    return result;
}
