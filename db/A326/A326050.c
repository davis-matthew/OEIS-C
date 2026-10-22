/*
 * A326050  a(n) = A082052(n) - n, where A082052 is the sum of divisors of n
 *          that are not of the form 4k+1.
 * https://oeis.org/A326050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A082052(n) - n * where A082052(n) = sum of the divisors d
 *            of n with d % 4 != 1
 * Range:     i8 n <= 95, i16 n <= 9492, i32 n <= 9492, i64 n <= 9492,
 *            i128 n <= 9492, big any n
 * Helpers:   add_divisors_not4k1, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/add_divisors_not4k1.h"

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;           /* n is small enough for uint64_t */
    /* Factor n */
    size_t maxf = 64;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n64, primes, exps, &cnt);

    /* Sum all required divisors */
    seq_t sum = 0;
    add_divisors_not4k1(primes, exps, 0, cnt, 1, &sum);

    /* Apply the final subtraction */
    return sum - (seq_t)n64;
}
