/*
 * A323898  Lexicographically earliest sequence such that a(i) = a(j) =>
 *          A000120(i) = A000120(j) and A083254(i) = A083254(j), for all i, j
 *          >= 1.
 * https://oeis.org/A323898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pairs
 *            [A000120(n), A083254(n)], computed by scanning k = 1..n and
 *            numbering each pair at its first appearance (A083254(n) =
 *            2*phi(n) - n)
 * Range:     u8 n <= 368, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 115 terms through n = 8713 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static long long *rg_pc = 0;   /* distinct pair: popcount */
static long long *rg_v = 0;    /* distinct pair: 2phi-n */
static size_t rg_distinct = 0, rg_dcap = 0;
static unsigned *rg_val = 0;   /* a(k) for k=1..rg_done, stored at index k */
static size_t rg_done = 0, rg_vcap = 0;

static long long rg_phi(uint64_t m)
{
    uint64_t r = m, x = m;
    uint64_t p;
    for (p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            r -= r / p;
        }
    }
    if (x > 1) r -= r / x;
    return (long long)r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > rg_done) {
        size_t k;
        if (n + 1 > rg_vcap) {
            size_t nc = rg_vcap ? rg_vcap : 1024;
            while (nc < n + 1) nc *= 2;
            rg_val = (unsigned *)realloc(rg_val, nc * sizeof(unsigned));
            rg_vcap = nc;
        }
        for (k = rg_done + 1; k <= n; k++) {
            long long pc = 0, v;
            uint64_t t = k;
            size_t i;
            while (t) { pc += (long long)(t & 1); t >>= 1; }
            v = 2 * rg_phi(k) - (long long)k;
            for (i = 0; i < rg_distinct; i++)
                if (rg_pc[i] == pc && rg_v[i] == v) break;
            if (i == rg_distinct) {
                if (rg_distinct == rg_dcap) {
                    rg_dcap = rg_dcap ? rg_dcap * 2 : 256;
                    rg_pc = (long long *)realloc(rg_pc, rg_dcap * sizeof(long long));
                    rg_v = (long long *)realloc(rg_v, rg_dcap * sizeof(long long));
                }
                rg_pc[i] = pc;
                rg_v[i] = v;
                rg_distinct++;
            }
            rg_val[k] = (unsigned)(i + 1);
        }
        rg_done = n;
    }
    return (seq_t)rg_val[n];
}
