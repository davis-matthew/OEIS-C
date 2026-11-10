/*
 * A350314  The catch-up points of the Redstone permutation A350313.
 * https://oeis.org/A350314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th n such that {A350313(1..n)} = {1..n}; found by
 *            simulating the Redstone permutation A350313 (a(1)=2, a(2)=1,
 *            then least unused k with gcd(k,m)=1 and k not dividing m-1) and
 *            recording each m where the maximum of the first m terms equals m
 * Range:     u8 n <= 42, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *rs_used = NULL;
static uint64_t rs_cap = 0;
static uint64_t rs_pos = 0, rs_max = 0, rs_low = 1;
static uint64_t *rs_cp = NULL;
static uint64_t rs_cpn = 0, rs_cpcap = 0;

static uint64_t rs_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void rs_ensure(uint64_t idx)
{
    if (idx < rs_cap) return;
    uint64_t nc = rs_cap ? rs_cap : 1024;
    while (nc <= idx) nc <<= 1;
    unsigned char *p = (unsigned char *)realloc(rs_used, (size_t)nc);
    if (!p) exit(1);
    for (uint64_t i = rs_cap; i < nc; ++i) p[i] = 0;
    rs_used = p;
    rs_cap = nc;
}

static void rs_step(void)
{
    uint64_t p = ++rs_pos;
    uint64_t k;
    if (p == 1) k = 2;
    else if (p == 2) k = 1;
    else {
        k = rs_low;
        for (;;) {
            rs_ensure(k);
            if (!rs_used[k] && rs_gcd(k, p) == 1 && ((p - 1) % k) != 0) break;
            ++k;
        }
    }
    rs_ensure(k);
    rs_used[k] = 1;
    for (;;) {
        rs_ensure(rs_low);
        if (!rs_used[rs_low]) break;
        ++rs_low;
    }
    if (k > rs_max) rs_max = k;
    if (rs_max == p) {
        if (rs_cpn == rs_cpcap) {
            uint64_t nc = rs_cpcap ? rs_cpcap * 2 : 256;
            uint64_t *q = (uint64_t *)realloc(rs_cp, (size_t)(nc * sizeof(uint64_t)));
            if (!q) exit(1);
            rs_cp = q;
            rs_cpcap = nc;
        }
        rs_cp[rs_cpn++] = p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (rs_cpn < (uint64_t)n) rs_step();
    return (seq_t)rs_cp[(uint64_t)n - 1];
}
