/*
 * A253501  Number of (7+1) X (n+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 400*3^(n-3) + 5274*2^(n-1) + 45170 for n > 6 Initial
 *            terms: a(1)=48293, a(2)=55362, a(3)=68992, a(4)=89339,
 *            a(5)=133284, a(6)=224747
 * Range:     u8 none, u16 n <= 2, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* first six terms hard-coded */
    static const seq_t init[6] = {
        (seq_t)48293,
        (seq_t)55362,
        (seq_t)68992,
        (seq_t)89339,
        (seq_t)133284,
        (seq_t)224747
    };

    if (n <= 6)
        return init[n - 1];

    /* compute the two powers */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)(n - 3));
    seq_t pow2 = big_pow((seq_t)2, (seq_t)(n - 1));

    /* evaluate formula */
    seq_t result = (seq_t)400 * pow3 + (seq_t)5274 * pow2 + (seq_t)45170;
    return result;
}
