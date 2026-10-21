/*
 * A286574  Sum of the binary weights of the lengths of 1-runs in base-2
 *          representation of n: a(n) = A000523(A286575(n)).
 * https://oeis.org/A286574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of popcounts of the lengths of 1‑runs in the binary
 *            expansion of n
 * Range:     u8 n <= 9486, u16 n <= 9486, u32 n <= 9486, u64 n <= 9486,
 *            u128 n <= 9486, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t run = 0;

    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) {
            run++;
        } else if (run) {
            sum += (seq_t)__builtin_popcountll((unsigned long long)run);
            run = 0;
        }
        tmp >>= 1;
    }
    if (run)
        sum += (seq_t)__builtin_popcountll((unsigned long long)run);

    return sum;
}
