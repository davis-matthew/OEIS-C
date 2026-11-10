/*
 * A328529  G.f. = Phi^4, where Phi = g.f. for A028930.
 * https://oeis.org/A328529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = Phi^4 where Phi = Sum_{x,y in Z} q^(2x^2+x*y+3y^2)
 *            (A028930); computed by direct counting and truncated convolution
 * Range:     u8 n <= 7, u16 n <= 51, u32 n <= 2003, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 59 terms through n = 3826 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *t = mem_new(seq_t, N + 1);
    seq_t *s = mem_new(seq_t, N + 1);

    long long nn = (long long)n;
    long long X = 0, Y = 0;
    while (23LL * (X + 1) * (X + 1) <= 12LL * nn) X++;
    while (23LL * (Y + 1) * (Y + 1) <= 8LL * nn) Y++;

    for (long long y = -Y; y <= Y; ++y) {
        for (long long x = -X; x <= X; ++x) {
            long long v = 2LL * x * x + x * y + 3LL * y * y;
            if (v >= 0 && v <= nn) t[(size_t)v] += 1;
        }
    }

    for (size_t k = 0; k <= N; ++k) {
        seq_t acc = 0;
        for (size_t i = 0; i <= k; ++i) {
            acc += t[i] * t[k - i];
        }
        s[k] = acc;
    }

    seq_t res = 0;
    for (size_t i = 0; i <= N; ++i) {
        res += s[i] * s[N - i];
    }

    mem_free(t);
    mem_free(s);
    return res;
}
