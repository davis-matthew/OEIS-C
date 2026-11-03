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
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   factor_u64, sigma_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_u64.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

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
    seq_t sigma = sigma_u64(m);

    /* 5. Result = fact * two_pow * sigma */
    seq_t result = fact;
    result *= two_pow;
    result *= sigma;

    return result;
}
