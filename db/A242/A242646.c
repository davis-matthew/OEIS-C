/*
 * A242646  a(n) = concatenation of decimal expansions of powers of 11 (in
 *          increasing order).
 * https://oeis.org/A242646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of 11^k for k = 0..n
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 41 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Computes the nth term of OEIS sequence A242646.
 * a(0) = 1
 * a(1) = 111
 * a(2) = 111121
 * ...
 * a(n) = concat( 11^0, 11^1, …, 11^n )
 */
seq_t a(idx_t n)
{
    seq_t result = 0;     /* cumulative concatenated value */
    seq_t power  = 1;     /* current 11^k */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* number of decimal digits in power */
        int d = num_digits(power);

        /* shift result left by d decimal digits: result *= 10^d */
        seq_t ten_pow = pow10((idx_t)d);
        result = result * ten_pow + power;

        /* prepare next power of 11, unless this was the last iteration */
        if (k < n)
            power = power * 11;
    }
    return result;
}
