/*
 * A353134  a(n) = PrimePi(n*SumOfDigits(n)).
 * https://oeis.org/A353134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = PrimePi(n * SumOfDigits(n))
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
 * Helpers:   digit_sum, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* sum of the decimal digits of n */
    seq_t s = digit_sum((seq_t)n);

    /* product n * SumOfDigits(n) */
    seq_t prod = ((seq_t)n) * s;

    /* compute_pi expects an idx_t argument */
    idx_t limit = (idx_t)prod;

    return prime_pi(limit);
}
