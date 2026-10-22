/*
 * A322392  Array A read by antidiagonals: A(n,k) = n-th digit of the base k
 *          expansion of 1/n.
 * https://oeis.org/A322392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   nth_digit_of_fraction
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th entry when the array
   A(i,j) = i‑th digit of the base j expansion of 1/i
   is read by antidiagonals, with a zig‑zag reversal on even
   diagonals.  For each n≥1 the function returns that digit. */

#include <stdint.h>
#include "../utils/nth_digit_of_fraction.h"

/* main entry for the OEIS sequence A322392 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defined for n≥1 */

    /* find the diagonal sum s such that (s-1)*s/2 ≥ n  */
    idx_t lo = 2, hi = n + 1;        /* upper bound always enough */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t val = (seq_t)(mid - 1) * mid / 2;
        if (val >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t s = lo;                     /* diagonal index (i+j) */

    /* count of entries before this diagonal */
    seq_t prev = (seq_t)(s - 2) * (s - 1) / 2;

    /* position within the diagonal (1‑based) */
    idx_t t = n - (idx_t)prev;        /* 1 … s-1 */

    /* mapping with zig‑zag: odd diagonals increasing i,
       even diagonals decreasing i */
    idx_t i, j;
    if (s & 1) {                     /* odd s */
        i = t;
        j = s - t;
    } else {                          /* even s */
        i = s - t;
        j = t;
    }

    return nth_digit_of_fraction(i, j, i);
}
