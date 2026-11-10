/*
 * A369277  Distinct values of A369317, in order of appearance.
 * https://oeis.org/A369277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th distinct value of A369317(m) = A091255(m, m+1)
 *            (GF(2) polynomial gcd), in order of first appearance; computed
 *            by scanning m = 1, 2, 3,
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A369277_CAP 4096

static unsigned A369277_bitlen(uint64_t x)
{
    unsigned l = 0;
    while (x) { x >>= 1; ++l; }
    return l;
}

static uint64_t A369277_polygcd(uint64_t a, uint64_t b)
{
    while (b) {
        unsigned db = A369277_bitlen(b);
        unsigned da = A369277_bitlen(a);
        while (a && da >= db) {
            a ^= b << (da - db);
            da = A369277_bitlen(a);
        }
        uint64_t t = a; a = b; b = t;
    }
    return a;
}

static uint64_t A369277_vals[A369277_CAP];
static unsigned A369277_cnt = 0;
static uint64_t A369277_next_m = 1;

seq_t a(idx_t n)
{
    while (A369277_cnt < n && A369277_cnt < A369277_CAP) {
        uint64_t m = A369277_next_m++;
        uint64_t g = A369277_polygcd(m, m + 1);
        bool found = false;
        for (unsigned i = 0; i < A369277_cnt; i++) {
            if (A369277_vals[i] == g) { found = true; break; }
        }
        if (!found) A369277_vals[A369277_cnt++] = g;
    }
    if (n < 1 || n > A369277_cnt) return (seq_t)0;
    return (seq_t)A369277_vals[n - 1];
}
