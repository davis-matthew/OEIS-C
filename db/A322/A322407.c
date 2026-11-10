/*
 * A322407  Compound sequence a(n) = A319198(A278039(n)), for n >= 0.
 * https://oeis.org/A322407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*B(n) - A(n) - (n - 1), where B = A278039 (tribonacci
 *            representation of n with a 0 appended) and A = A278040
 *            (tribonacci representation of n with 0,1 appended)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Tribonacci weights 1, 2, 4, 7, 13, ... kept in native 64-bit ints. */
    uint64_t w[100];
    size_t len = 3;
    w[0] = 1; w[1] = 2; w[2] = 4;
    /* extend until w[len-3] > n, so w[j+2] exists for every digit j used */
    while (w[len - 3] <= (uint64_t)n && len < 100) {
        w[len] = w[len - 1] + w[len - 2] + w[len - 3];
        len++;
    }

    uint64_t rem = (uint64_t)n;
    seq_t B = 0;   /* A278039(n): shift digits by one place  */
    seq_t A = 1;   /* A278040(n): shift by two places, plus 1 */
    for (size_t j = len - 2; j-- > 0; ) {
        if (rem >= w[j]) {
            rem -= w[j];
            B += (seq_t)w[j + 1];
            A += (seq_t)w[j + 2];
        }
    }

    /* 3*B + 1 - A = a(n) + n >= 0, then subtract n */
    seq_t res = B * 3 + 1;
    res -= A;
    res -= (seq_t)n;
    return res;
}
