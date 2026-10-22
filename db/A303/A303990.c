/*
 * A303990  Triangle, read by rows: n^k * k^n, for n >= 1 and k = 1..n.
 * https://oeis.org/A303990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (m^k) * (k^m) where m = the row number such that m(m-1)/2
 *            < n <= m(m+1)/2 k = n - m(m-1)/2 (1‑based column in the row)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 26, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small exponentiation by squaring.  The exponent exp is an
   idx_t (uint64_t); the base and result are seq_t.  For the
   64‑bit build the intermediate products fit within the allowed
   range of the test cases. */
static seq_t pow_seq_simple(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row m such that n lies between the cumulative
       triangular numbers.  The loop runs at most about sqrt(2n)
       times (≈141 for n = 10 000), which is negligible. */
    idx_t m = 1;
    while (m * (m + 1) / 2 < n) ++m;

    idx_t prev_tri = m * (m - 1) / 2;           /* terms before this row */
    idx_t k = n - prev_tri;                    /* 1-based column in the row */

    seq_t mn = (seq_t)m;
    seq_t kn = (seq_t)k;

    seq_t part1 = pow_seq_simple(mn, k);        /* m^k */
    seq_t part2 = pow_seq_simple(kn, m);        /* k^m */

    return part1 * part2;
}
