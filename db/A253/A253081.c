/*
 * A253081  Partial sums of A246029.
 * https://oeis.org/A253081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A246029(k), where A246029(k) = product over
 *            maximal runs of 1's in binary k of prime(run length),
 *            A246029(0)=1
 * Range:     u8 n <= 45, u16 n <= 2275, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 78 terms through n = 7970 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t A246029_prime(unsigned k)
{
    static uint32_t pr[70];
    static unsigned cnt = 0;
    if (cnt == 0) {
        uint32_t c = 2;
        while (cnt < 70) {
            bool ok = true;
            for (unsigned i = 0; i < cnt; ++i) {
                if ((uint64_t)pr[i] * pr[i] > c) break;
                if (c % pr[i] == 0) { ok = false; break; }
            }
            if (ok) pr[cnt++] = c;
            c++;
        }
    }
    return pr[k - 1];
}

static seq_t A246029_a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t prod = (seq_t)1;
    idx_t x = n;
    while (x) {
        while ((x & 1) == 0) x >>= 1;
        unsigned run = 0;
        while (x & 1) { run++; x >>= 1; }
        prod *= (seq_t)A246029_prime(run);
    }
    return prod;
}

seq_t a(idx_t n)
{
    seq_t s = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k)
        s += A246029_a(k);
    return s;
}
