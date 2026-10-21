/*
 * A253449  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 400*3^(n-3) + 205*2^(n-1) + 2917 for n > 7 a(1)=81,
 *            a(2)=414, a(3)=1388, a(4)=3639, a(5)=8501, a(6)=19701,
 *            a(7)=48293
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

static const seq_t small[7] = {
    (seq_t)81,
    (seq_t)414,
    (seq_t)1388,
    (seq_t)3639,
    (seq_t)8501,
    (seq_t)19701,
    (seq_t)48293
};

seq_t a(idx_t n)
{
    if (n <= 7) {
        /* Offset is 1, so index 1 maps to small[0] */
        return small[n-1];
    }

    /* Exponents are at most 10000, fits in int */
    int exp3 = (int)(n - 3);
    int exp2 = (int)(n - 1);

    seq_t pow3 = pow_seq((seq_t)3, exp3);
    seq_t pow2 = pow_seq((seq_t)2, exp2);

    return (seq_t)400 * pow3 + (seq_t)205 * pow2 + (seq_t)2917;
}
