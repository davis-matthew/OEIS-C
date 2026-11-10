/*
 * A280494  Rows of triangular table A280499 read in reverse order.
 * https://oeis.org/A280494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = quotient of r by k in GF(2)[X] (carryless division), or 0
 *            if k does not divide r, where r is the row (r(r-1)/2 < n <=
 *            r(r+1)/2) and k = r - p + 1 with p = n - r(r-1)/2 (rows of
 *            A280499 reversed)
 * Range:     u8 n <= 8256, u16 n <= 8256, u32 n <= 8256, u64 n <= 8256,
 *            u128 n <= 8256, big any n
 * Validated: standard, pass, 136 terms through n = 7598 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A280494_deg(uint64_t x)
{
    int d = -1;
    while (x) { d++; x >>= 1; }
    return d;
}

static uint64_t A280494_gf2_div(uint64_t dividend, uint64_t divisor)
{
    if (divisor == 0) return 0;
    uint64_t q = 0, rem = dividend;
    int dd = A280494_deg(divisor);
    while (rem != 0) {
        int dr = A280494_deg(rem);
        if (dr < dd) break;
        int s = dr - dd;
        q |= ((uint64_t)1 << s);
        rem ^= (divisor << s);
    }
    return rem == 0 ? q : 0;
}

seq_t a(idx_t n)
{
    uint64_t r = 1, sum = 1;
    while (sum < (uint64_t)n) {
        ++r;
        sum += r;
    }
    uint64_t p = (uint64_t)n - (sum - r);
    uint64_t k = r - p + 1;
    return (seq_t)A280494_gf2_div(r, k);
}
