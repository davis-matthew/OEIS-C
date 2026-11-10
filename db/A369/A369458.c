/*
 * A369458  Arithmetic derivative of the odd part of n, permuted by A163511
 *          ("Doudna-sequence mirrored").
 * https://oeis.org/A369458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A369465(A163511(n)) = A003415(odd part of A163511(n));
 *            A163511(n) is built from its known prime factorization, so no
 *            factoring is needed
 * Range:     u8 n <= 32, u16 n <= 258, u32 n <= 8206, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 96 terms through n = 9724 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* first 64 primes by trial division */
    uint32_t pr[64];
    int np = 0;
    for (uint32_t c = 2; np < 64; ++c) {
        int ok = 1;
        for (int j = 0; j < np && (uint64_t)pr[j] * pr[j] <= c; ++j)
            if (c % pr[j] == 0) { ok = 0; break; }
        if (ok) pr[np++] = c;
    }

    /* exponents of prime i (0-based) in A163511(n) */
    uint32_t ex[64];
    int cnt = 0;
    idx_t prev = 0, bitpos = 1, tmp = n;
    while (tmp) {
        if (tmp & 1) {
            ex[cnt++] = (uint32_t)(bitpos - prev - 1);
            prev = bitpos;
        }
        ++bitpos;
        tmp >>= 1;
    }
    ex[cnt - 1] += 1; /* extra factor p(r) */

    /* odd part: ignore prime 2 (index 0) */
    seq_t m = 1;
    for (int i = 1; i < cnt; ++i)
        for (uint32_t e = 0; e < ex[i]; ++e)
            m *= (seq_t)pr[i];

    seq_t res = 0;
    for (int i = 1; i < cnt; ++i) {
        if (ex[i] == 0) continue;
        res += (m / (seq_t)pr[i]) * (seq_t)ex[i];
    }
    return res;
}
