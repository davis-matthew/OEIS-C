/*
 * A249742  Inverse permutation to A249741.
 * https://oeis.org/A249742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ((((x+y)^2) - x - 3*y)/2), x = A055396(n+1), y =
 *            A078898(n+1)
 * Range:     u8 n <= 46, u16 n <= 724, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 92 terms through n = 7726 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n + 1;
    uint32_t *spf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    if (!spf) return (seq_t)0;

    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            for (size_t j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    uint32_t p = spf[N];
    uint64_t x = 0; /* index of smallest prime factor of N */
    uint64_t y = 0; /* count of m <= N with the same smallest prime factor */
    for (size_t i = 2; i <= N; ++i) {
        if (spf[i] == (uint32_t)i && i <= p) ++x;
        if (spf[i] == p) ++y;
    }
    free(spf);

    seq_t X = (seq_t)x, Y = (seq_t)y;
    seq_t S = X + Y;
    return (seq_t)1 + (S * S - X - (seq_t)3 * Y) / 2;
}
