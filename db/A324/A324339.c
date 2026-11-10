/*
 * A324339  a(n) = A246707(A005940(1+n)), where A005940 is the Doudna
 *          sequence, and A246707 is the expansion of phi(-q) * phi(-q^2) *
 *          phi(-q^3) * phi(-q^6) in powers of q.
 * https://oeis.org/A324339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A246707(A005940(1+n)); A246707(N) = coefficient of q^N in
 *            phi(-q)*phi(-q^2)*phi(-q^3)*phi(-q^6), where phi(-q) = 1 +
 *            2*Sum_{j>=1} (-1)^j q^(j^2). Computed as a pair-wise convolution
 *            over a^2+2b^2 and 3c^2+6d^2
 * Range:     i8 n <= 26, i16 n <= 468, i32 n <= 1505, i64 n <= 1505,
 *            i128 n <= 1505, big any n
 * Validated: standard, pass, 97 terms through n = 1480 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t A005940_u64(uint64_t n)
{
    static const uint64_t pr[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    uint64_t m = n - 1, r = 1;
    int i = 1;
    while (m > 0) {
        if (m & 1) { r *= pr[i - 1]; m = (m - 1) / 2; }
        else { m /= 2; i++; }
    }
    return r;
}

static seq_t A324339_w(uint64_t j)
{
    if (j == 0) return (seq_t)1;
    return (j & 1) ? (seq_t)(-2) : (seq_t)2;
}

static seq_t A246707_a(uint64_t N)
{
    if (N == 0) return (seq_t)1;
    seq_t *g = mem_new(seq_t, (size_t)(N + 1));
    if (!g) return (seq_t)0;
    for (uint64_t c = 0; 3 * c * c <= N; ++c) {
        for (uint64_t d = 0; 3 * c * c + 6 * d * d <= N; ++d) {
            g[3 * c * c + 6 * d * d] += A324339_w(c) * A324339_w(d);
        }
    }
    seq_t total = 0;
    for (uint64_t a = 0; a * a <= N; ++a) {
        for (uint64_t b = 0; a * a + 2 * b * b <= N; ++b) {
            uint64_t s = a * a + 2 * b * b;
            total += A324339_w(a) * A324339_w(b) * g[N - s];
        }
    }
    mem_free(g);
    return total;
}

seq_t a(idx_t n)
{
    uint64_t N = A005940_u64((uint64_t)n + 1);
    return A246707_a(N);
}
