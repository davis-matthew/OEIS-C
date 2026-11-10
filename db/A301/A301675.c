/*
 * A301675  Partial sums of A301674.
 * https://oeis.org/A301675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} b(k), where b = A301674: b(n) = p(n) -
 *            b(n-1) + 2*b(n-3) + 2*b(n-4) - b(n-6) - b(n-7), p(n) =
 *            [1,5,12,20,20,18,5,1,-2] for n=0..8, p(n)=0 for n>=9 (b(k)=0 for
 *            k<0)
 * Range:     u8 n <= 10, u16 n <= 171, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 883 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static const int p[9] = {1, 5, 12, 20, 20, 18, 5, 1, -2};
    size_t len = (size_t)n + 1;
    seq_t *b = mem_new(seq_t, len);
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        seq_t v = 0;
        if (i < 9) {
            if (p[i] >= 0) v += (seq_t)p[i];
            else v -= (seq_t)(-p[i]);
        }
        if (i >= 1) v -= b[i-1];
        if (i >= 3) v += 2 * b[i-3];
        if (i >= 4) v += 2 * b[i-4];
        if (i >= 6) v -= b[i-6];
        if (i >= 7) v -= b[i-7];
        b[i] = v;
        sum += v;
    }
    mem_free(b);
    return sum;
}
