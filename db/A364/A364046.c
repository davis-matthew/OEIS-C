/*
 * A364046  Expansion of Sum_{k>0} x^(4*k) / (1 + x^(5*k)).
 * https://oeis.org/A364046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d==4 (mod 5)} (-1)^d
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   _sum_divs, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/_sum_divs.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1; behaviour for 0 not required */
        return 0;

    /* Factor n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the sum over divisors */
    return _sum_divs(primes, exps, cnt, 0, 1ULL);
}
