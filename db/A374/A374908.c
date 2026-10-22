/*
 * A374908  Each term is the sum of the preceding term and its seven-segment
 *          total A006942.
 * https://oeis.org/A374908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + A006942(a(n-1)), a(0) = 0
 * Range:     u8 n <= 23, u16 n <= 2870, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where A006942(x) is the sum of seven‑segment counts of the decimal digits of x
   (digits 0…9 use counts {6,2,5,5,4,5,6,3,7,6}) */

#include <stdbool.h>   /* required type for bool, not used here but safe */

/* Segment counts for digits 0–9 */
static const unsigned char _seg[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

/* Compute the seven‑segment total for a value */
static seq_t _segments(seq_t v)
{
    if (v == (seq_t)0)
        return (seq_t)6;               /* 0 uses 6 segments */

    seq_t sum = (seq_t)0;
    while (v > (seq_t)0) {
        unsigned d = (unsigned)(v % (seq_t)10); /* numeric digit */
        sum += (seq_t)_seg[d];
        v /= (seq_t)10;
    }
    return sum;
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)0;

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        val += _segments(val);  /* val is a(i-1) */
    }
    return val;                  /* a(n) */
}
