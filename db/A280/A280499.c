/*
 * A280499  Triangular table for division in ring GF(2)[X]: T(n,k) = n/k, or 0
 *          if k is not a divisor of n, where the binary expansion of each
 *          number defines the corresponding (0,1)-polynomial.
 * https://oeis.org/A280499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl, look
 * Formula:   Let r = ⌈(√(8n + 1) – 1)/2⌉ (row index) Let k = n – r(r – 1)/2
 *            (column index) a(n) = quotient of r divided by k in GF(2)[X], 0
 *            if k does not divide r. The division uses carryless arithmetic:
 *            the quotient is the binary representation of the polynomial
 *            quotient
 * Range:     u8 n <= 32640, u16 n <= 32896, u32 n <= 32896, u64 n <= 32896,
 *            u128 n <= 32896, big any n
 * Validated: standard, pass, 136 terms through n = 9993 (2026-11-01)
 * Helpers:   bit_length
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_length.h"

/* carryless division of dividend by divisor in GF(2)[X];
   returns quotient if divisor divides dividend, otherwise 0. */
static seq_t gf2_div(seq_t dividend, seq_t divisor)
{
    if (divisor == 0) return 0;   /* undefined, but safe */

    seq_t quotient = 0;
    seq_t rem = dividend;

    while (rem != 0) {
        unsigned int deg_rem = bit_length(rem) - 1;
        unsigned int deg_div = bit_length(divisor) - 1;

        if (deg_rem < deg_div) break;

        unsigned int shift = deg_rem - deg_div;
        quotient |= ((seq_t)1 << shift);
        rem ^= (divisor << shift);
    }

    return (rem == 0) ? quotient : 0;
}

seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t sum = 1;                /* r*(r+1)/2 for r=1 */

    while (sum < n) {
        ++r;
        sum += r;
    }

    idx_t prevSum = sum - r;   /* sum of first (r-1) rows */
    idx_t k = n - prevSum;     /* column index in the r-th row */

    seq_t dividend = (seq_t)r;
    seq_t divisor  = (seq_t)k;

    return gf2_div(dividend, divisor);
}
