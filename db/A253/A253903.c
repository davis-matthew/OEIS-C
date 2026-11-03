/*
 * A253903  The characteristic function of square pyramidal numbers.
 * https://oeis.org/A253903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a square pyramidal number of the form
 *            m(m+1)(2m+1)/6 for some integer m≥0; otherwise a(n) = 0
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 8517 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the square pyramidal number for a given m. */
static seq_t square_pyramidal(idx_t m)
{
    seq_t a = m;          /* m          */
    seq_t b = m + 1;      /* m + 1      */
    seq_t c = 2 * m + 1;  /* 2m + 1     */

    /* Divide one of a, b, c by 2 (exactly one is even). */
    if ((a & 1) == 0) {
        a /= 2;
    } else if ((b & 1) == 0) {
        b /= 2;
    } else {
        c /= 2;
    }

    /* Divide one of a, b, c by 3 (exactly one is divisible by 3). */
    if ((a % 3) == 0) {
        a /= 3;
    } else if ((b % 3) == 0) {
        b /= 3;
    } else {
        c /= 3;
    }

    /* The product now equals m(m+1)(2m+1)/6 and fits in seq_t. */
    return a * b * c;
}

/* Return the characteristic value at index n. */
seq_t a(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t val = square_pyramidal(mid);
        if (val == n) {
            return 1;
        } else if (val < n) {
            lo = mid + 1;
        } else {
            if (mid == 0)  /* prevents infinite loop when mid==0 */
                break;
            hi = mid - 1;
        }
    }
    return 0;
}
