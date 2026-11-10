/*
 * A322410  Compound tribonacci sequence with a(n) = A278040(A278039(n)), for
 *          n >= 0.
 * https://oeis.org/A322410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A278040(A278039(n)); with n = Sum d_i*w_i in greedy
 *            tribonacci representation (w = 1,2,4,7,13,...; w_k =
 *            w_{k-1}+w_{k-2}+w_{k-3}), a(n) = 1 + Sum d_i*w_{i+3}
 * Range:     u8 n <= 41, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t CAP = 200;
    seq_t *w = mem_new(seq_t, CAP);
    seq_t N = (seq_t)n;
    size_t m, j;
    seq_t rem, res;

    w[0] = 1;
    w[1] = 2;
    w[2] = 4;
    for (j = 3; j < CAP; ++j)
        w[j] = w[j - 1] + w[j - 2] + w[j - 3];

    /* m = smallest index with w[m] > N (count of usable weights) */
    m = 0;
    while (m < 80 && w[m] <= N)
        ++m;

    rem = N;
    res = 1;
    for (j = m; j-- > 0; ) {
        if (rem >= w[j]) {
            rem -= w[j];
            res += w[j + 3];
        }
    }
    mem_free(w);
    return res;
}
