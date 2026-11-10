/*
 * A277680  Inverse of the permutation A277679 of the natural numbers.
 * https://oeis.org/A277680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = inverse permutation of A277679, found by simulating the
 *            steps: at step k, if the 0-based index p < k, the term is
 *            removed there and a(n) = k(k-1)/2 + p + 1. Otherwise the
 *            remaining terms are reversed in blocks of k+1 and p moves to its
 *            new position
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9708 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t p = (seq_t)n - 1;
    seq_t step = 1;
    for (;;) {
        if (p < step) {
            return step * (step - 1) / 2 + p + 1;
        }
        seq_t q = p - step;
        seq_t B = step + 1;
        seq_t blk = q / B;
        seq_t r = q % B;
        p = blk * B + (B - 1 - r);
        step++;
    }
}
