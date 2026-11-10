/*
 * A369028  Exponential of Mangoldt function permuted by A253563.
 * https://oeis.org/A369028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014963(A253563(n)); A253563(0)=1, A253563(1)=2,
 *            A253563(2n)=A253560(A253563(n)),
 *            A253563(2n+1)=A253550(A253563(n)). Computed on prime-exponent
 *            vectors
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9921 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    int primes[128];
    int e[130];
    int i, cnt = 0, top = 0, bits = 0;
    if (n == 0) return (seq_t)1;

    for (int c = 2; cnt < 128; ++c) {
        int ok = 1;
        for (int d = 0; d < cnt && primes[d] * primes[d] <= c; ++d)
            if (c % primes[d] == 0) { ok = 0; break; }
        if (ok) primes[cnt++] = c;
    }
    for (i = 0; i < 130; ++i) e[i] = 0;
    e[0] = 1; /* value 2 at n=1 */

    {
        idx_t t = n;
        while (t > 1) { t >>= 1; ++bits; }
    }
    for (int b = bits - 1; b >= 0; --b) {
        int odd = (int)((n >> b) & 1);
        if (!odd) {
            e[top]++;
        } else {
            e[top]--;
            top++;
            e[top]++;
        }
    }
    {
        int nz = 0, idx = -1;
        for (i = 0; i < 130; ++i)
            if (e[i] > 0) { ++nz; idx = i; }
        if (nz == 1) return (seq_t)primes[idx];
    }
    return (seq_t)1;
}
