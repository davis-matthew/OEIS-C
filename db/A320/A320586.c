/*
 * A320586  Expansion of (1/(1 - x)) * Sum_{k>=1} k*x^k/(x^k + (1 - x)^k).
 * https://oeis.org/A320586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k)*A000593(k), A000593(k) = sum
 *            of odd divisors of k
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 938 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    seq_t *s = mem_new(seq_t, N + 1);
    seq_t *row = mem_new(seq_t, N + 1);
    for (size_t d = 1; d <= N; d += 2) {
        for (size_t m = d; m <= N; m += d) {
            s[m] += (seq_t)d;
        }
    }
    row[0] = 1;
    for (size_t i = 1; i <= N; i++) {
        for (size_t j = i; j >= 1; j--) {
            row[j] += row[j - 1];
        }
    }
    seq_t result = 0;
    for (size_t k = 1; k <= N; k++) {
        result += row[k] * s[k];
    }
    mem_free(s);
    mem_free(row);
    return result;
}
