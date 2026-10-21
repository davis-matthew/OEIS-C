/*
 * A257082  a(n) is one sixth of the total number of free ends of 6 line
 *          segments expansion at n iterations (see Comments lines for
 *          definition).
 * https://oeis.org/A257082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n-1)*2 + 2^(n-5) for n >= 9; initial values: a(1)=1,
 *            a(2)=2, a(3)=8, a(4)=12, a(5)=28, a(6)=52, a(7)=108, a(8)=216,
 *            a(9)=448
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* precomputed values for n = 1 .. 9 */
    static const seq_t init[10] = {
        0,          /* dummy for zero index */
        (seq_t)1,   /* a(1) */
        (seq_t)2,   /* a(2) */
        (seq_t)8,   /* a(3) */
        (seq_t)12,  /* a(4) */
        (seq_t)28,  /* a(5) */
        (seq_t)52,  /* a(6) */
        (seq_t)108, /* a(7) */
        (seq_t)216, /* a(8) */
        (seq_t)448  /* a(9) */
    };

    if (n <= 9)
        return init[n];

    /* start from a(9) and iterate according to the recurrence */
    seq_t value = init[9];
    seq_t pow2  = (seq_t)16; /* 2^(9-5) = 16 */

    for (idx_t i = 9; i < n; ++i) {
        value = value * (seq_t)2 + pow2;
        pow2  = pow2 * (seq_t)2;   /* advance 2^(i+1-5) */
    }

    return value;
}
