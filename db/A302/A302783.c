/*
 * A302783  A divisor-or-multiple permutation of natural numbers: a(n) =
 *          A052330(A006068(n)).
 * https://oeis.org/A302783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A052330(A006068(n)); A006068 = Gray-code inverse (n XOR
 *            n/2 XOR n/4 ...), A052330(m) = product of Fermi-Dirac primes
 *            (A050376) b_i over set bits i of m
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 1023, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 89 terms through n = 8047 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned fd_tab[64];
static int fd_ready = 0;

static void fd_build(void)
{
    unsigned cnt = 0;
    unsigned c;
    for (c = 2; cnt < 64; ++c) {
        unsigned x = c, p = 0, e = 0, d;
        for (d = 2; d * d <= x; ++d) {
            if (x % d == 0) { p = d; break; }
        }
        if (p == 0) p = x;
        while (x % p == 0) { x /= p; ++e; }
        if (x == 1 && (e & (e - 1)) == 0)
            fd_tab[cnt++] = c;
    }
    fd_ready = 1;
}

seq_t a(idx_t n)
{
    if (!fd_ready) fd_build();
    uint64_t m = (uint64_t)n, s = (uint64_t)n;
    while (s > 0) {
        s >>= 1;
        m ^= s;
    }
    seq_t res = 1;
    for (unsigned i = 0; i < 64; ++i) {
        if ((m >> i) & 1u)
            res *= (seq_t)fd_tab[i];
    }
    return res;
}
