/*
 * A327029  T(n, k) = Sum_{d|n} phi(d) * A008284(n/d, k) for n >= 1, T(0, 0) =
 *          1. Triangle read by rows for 0 <= k <= n.
 * https://oeis.org/A327029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{d|n} phi(d)*A008284(n/d,k) for n>=1, T(0,0)=1;
 *            a(n) is the row-major flattening of the triangle, 0<=k<=n
 * Range:     u8 n <= 356, u16 n <= 1840, u32 n <= 12742, u64 n <= 20300,
 *            u128 n <= 20300, big any n
 * Validated: standard, pass, 107 terms through n = 9649 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static unsigned long A327029_phi(unsigned long m)
{
    unsigned long r = m, x = m;
    for (unsigned long p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            r -= r / p;
        }
    }
    if (x > 1) r -= r / x;
    return r;
}

seq_t a(idx_t n)
{
    unsigned long r = 0;
    while ((unsigned long)(((idx_t)r + 1) * ((idx_t)r + 2) / 2) <= (unsigned long)n) r++;
    unsigned long k = (unsigned long)(n - (idx_t)(r * (r + 1) / 2));
    if (r == 0) return (seq_t)1;
    if (k == 0) return (seq_t)0;

    size_t w = (size_t)r + 1;
    seq_t *P = mem_new(seq_t, w * w);
    P[0] = 1;
    for (size_t m = 1; m <= r; ++m) {
        for (size_t j = 1; j <= m; ++j) {
            seq_t v = P[(m - 1) * w + (j - 1)];
            v += P[(m - j) * w + j];
            P[m * w + j] = v;
        }
    }
    seq_t sum = 0;
    for (unsigned long d = 1; d <= r; ++d) {
        if (r % d == 0) {
            sum += (seq_t)A327029_phi(d) * P[(size_t)(r / d) * w + k];
        }
    }
    mem_free(P);
    return sum;
}
