/*
 * A279610  a(n) = concatenate n consecutive integers, starting with the last
 *          number of the previous batch.
 * https://oeis.org/A279610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the n consecutive integers starting from
 *            start(n) = 1 + (n-2)(n-1)/2. Thus a(1)=1, a(2)=12, a(3)=234, …
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

static inline int digit_count_seq(seq_t x)
{
    int d = 0;
    seq_t t = x;
    do {
        t /= 10;
        d++;
    } while (t > 0);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* start = 1 + (n-2)(n-1)/2 */
    seq_t start = ((seq_t)(n - 2) * (seq_t)(n - 1)) / 2 + 1;

    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t cur   = start + (seq_t)i;
        int   digs  = digit_count_seq(cur);
        seq_t mul   = pow10((idx_t)digs);
        result = result * mul + cur;
    }
    return result;
}
