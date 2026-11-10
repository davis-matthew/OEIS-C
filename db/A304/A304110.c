/*
 * A304110  a(n) is the number of terms of A304107 <= n; Partial sums of
 *          A304109.
 * https://oeis.org/A304110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A304109(k), where A304109(k)=1 iff the
 *            GF(2)[x] polynomial with binary expansion k is squarefree
 *            (gcd(f,f')=1); computed by direct counting
 * Range:     u8 n <= 508, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 102 terms through n = 7832 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int A304110_deg(uint64_t x)
{
    int d = -1;
    while (x) { d++; x >>= 1; }
    return d;
}

static bool A304110_sqfree(uint64_t f)
{
    if (f == 1) return true;
    if (f == 0) return false;
    uint64_t d = (f >> 1) & 0x5555555555555555ULL;
    if (d == 0) return false;
    uint64_t p = f, q = d;
    while (q != 0) {
        int dq = A304110_deg(q);
        uint64_t r = p;
        while (r != 0) {
            int dr = A304110_deg(r);
            if (dr < dq) break;
            r ^= q << (dr - dq);
        }
        p = q;
        q = r;
    }
    return p == 1;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    for (uint64_t i = 1; i <= (uint64_t)n; ++i)
        if (A304110_sqfree(i)) count++;
    return (seq_t)count;
}
