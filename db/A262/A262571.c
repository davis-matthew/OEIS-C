/*
 * A262571  Concatenation of the numbers from 2 to n.
 * https://oeis.org/A262571
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of integers 2,3,…,n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 19 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t result = 0;

    for (idx_t k = 2; k <= n; ++k)
    {
        uint64_t d = num_digits0(k);
        result = result * pow10(d) + (seq_t)k;
    }

    return result;
}
