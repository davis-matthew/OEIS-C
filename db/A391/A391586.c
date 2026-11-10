/*
 * A391586  a(n) = n XOR A391571(n).
 * https://oeis.org/A391586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n XOR A391571(n); A391571(n) found by search for smallest
 *            odd k>1 with exact carryless base-2 division of n*k by k
 * Range:     u8 n <= 28, u16 n <= 7530, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 106 terms through n = 9652 (2026-11-10)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_length.h"

static seq_t A391571_carryless_div(seq_t dividend, seq_t divisor)
{
    seq_t rem = dividend;
    seq_t quo = 0;
    if (divisor == 0)
        return 0;
    int div_deg = (int)bit_length(divisor) - 1;
    if (div_deg < 0)
        return 0;
    while (rem != 0) {
        int rem_deg = (int)bit_length(rem) - 1;
        if (rem_deg < div_deg)
            break;
        int shift = rem_deg - div_deg;
        quo |= ((seq_t)1 << shift);
        rem ^= (divisor << shift);
    }
    return (rem == 0) ? quo : 0;
}

static seq_t A391571_a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sn = (seq_t)n;
    for (idx_t k = 3; ; k += 2) {
        seq_t prod = sn * (seq_t)k;
        seq_t q = A391571_carryless_div(prod, (seq_t)k);
        if (q != 0)
            return q;
    }
}

seq_t a(idx_t n)
{
    return (seq_t)n ^ A391571_a(n);
}
