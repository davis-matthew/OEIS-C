/*
 * A243521  The number of states in a Tower of Hanoi puzzle with three pegs
 *          and n discs, where a larger disc can be placed directly on top of
 *          a smaller one at most once per peg.
 * https://oeis.org/A243521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 3*n*5^(n-1) + 3*n*(n-1)*4^(n-2) -
 *            n*(n-1)*(n-2)*3^(n-3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t N = (seq_t)n;

    /* First term: 6^n */
    result = big_pow((seq_t)6, N);

    /* Second term: -3*n*5^(n-1) */
    if (n >= 1) {
        seq_t term = ( (seq_t)3 * N * big_pow((seq_t)5, (seq_t)(n-1)) );
        result -= term;
    }

    /* Third term: +3*n*(n-1)*4^(n-2) */
    if (n >= 2) {
        seq_t term = ( (seq_t)3 * N * (seq_t)(n-1) * big_pow((seq_t)4, (seq_t)(n-2)) );
        result += term;
    }

    /* Fourth term: -n*(n-1)*(n-2)*3^(n-3) */
    if (n >= 3) {
        seq_t term = ( N * (seq_t)(n-1) * (seq_t)(n-2) * big_pow((seq_t)3, (seq_t)(n-3)) );
        result -= term;
    }

    return result;
}
