/*
 * A341609  Characteristic function of A337372: a(n) = 1 if A337345(n) = 1,
 *          otherwise 0.
 * https://oeis.org/A341609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n >= 2, A003961(n) > 2n and A003961(n/p) <= 2n/p for
 *            every prime p | n (n is a primitive term of A246282), else 0;
 *            A003961 = replace each prime by the next prime
 * Range:     u8 n <= 53, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9447 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A341609_nextprime(uint64_t p)
{
    uint64_t q = p + 1;
    for (;; ++q) {
        bool pr = q >= 2;
        for (uint64_t d = 2; d * d <= q; ++d)
            if (q % d == 0) { pr = false; break; }
        if (pr) return q;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    uint64_t P[20], NP[20];
    int np = 0;
    uint64_t x = (uint64_t)n;
    seq_t f = 1;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p) continue;
        uint64_t q = A341609_nextprime(p);
        P[np] = p; NP[np] = q; ++np;
        while (x % p == 0) { x /= p; f *= (seq_t)q; }
    }
    if (x > 1) {
        uint64_t q = A341609_nextprime(x);
        P[np] = x; NP[np] = q; ++np;
        f *= (seq_t)q;
    }
    seq_t nn = (seq_t)n;
    if (!(f > 2 * nn)) return (seq_t)0;
    for (int i = 0; i < np; ++i) {
        seq_t g = f / (seq_t)NP[i];
        seq_t m = nn / (seq_t)P[i];
        if (g > 2 * m) return (seq_t)0;
    }
    return (seq_t)1;
}
