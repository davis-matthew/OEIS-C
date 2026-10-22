/*
 * A317195  Length of smallest linear covering word for covering (n-2)-subsets
 *          of an n-set.
 * https://oeis.org/A317195
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(3) = 3. For n >= 4: a(n) = n + floor((n-1)^2 / 2) + 2 * max(0,
 *            n-5) (Offset: 3, i.e., a(3) is the first term.)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 3) {
        return (seq_t)3;
    }

    seq_t n_st   = (seq_t)n;
    seq_t nm1    = n_st - (seq_t)1;
    seq_t sq     = nm1 * nm1;          /* (n-1)^2, fits in seq_t for n up to ~10^4 */
    seq_t half   = sq / (seq_t)2;      /* floor((n-1)^2 / 2) */

    seq_t result = n_st + half;        /* base term */

    if (n > 4) {
        result += (seq_t)(2 * (n_st - (seq_t)5));
    }

    return result;
}
