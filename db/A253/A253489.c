/*
 * A253489  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 49*3^(n-1) + 291*2^(n-1) + 1017 for n > 5 Initial values
 *            for n = 1..5: 414, 1377, 2640, 4720, 9654
 * Range:     u8 none, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    static const seq_t init[5] = {
        (seq_t)414,
        (seq_t)1377,
        (seq_t)2640,
        (seq_t)4720,
        (seq_t)9654
    };

    if (n <= 5) return init[n - 1];

    /* Compute 49*3^(n-1) + 291*2^(n-1) + 1017 */
    seq_t exp = (seq_t)(n - 1);
    seq_t p3 = big_pow((seq_t)3, exp);
    seq_t p2 = big_pow((seq_t)2, exp);

    seq_t res = p3 * (seq_t)49;
    res += p2 * (seq_t)291;
    res += (seq_t)1017;
    return res;
}
