/*
 * A393139  Row sums of A393138.
 * https://oeis.org/A393139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} C(n+1, i+1) * C(n-i, floor((n-i)/2))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 1918 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *row = mem_new(seq_t, N + 3);
    seq_t *mid = mem_new(seq_t, N + 2);
    row[0] = 1;
    mid[0] = 1;
    for (size_t m = 1; m <= N + 1; ++m) {
        for (size_t j = m; j >= 1; --j)
            row[j] = row[j] + row[j - 1];
        if (m <= N) mid[m] = row[m / 2];
    }
    /* row now holds C(n+1, .) */
    seq_t sum = 0;
    for (size_t i = 0; i <= N; ++i)
        sum += row[i + 1] * mid[N - i];
    mem_free(row);
    mem_free(mid);
    return sum;
}
