/*
 * A303492  Numbers m such that m divides A302205(m).
 * https://oeis.org/A303492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th m >= 1 such that m divides A302205(m); found by a
 *            search over m, with A302205(m) mod m computed by modular
 *            concatenation
 * Range:     u8 n <= 40, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A303492_check(uint64_t m)
{
    typedef unsigned __int128 u128;
    static bool init = false;
    static unsigned digs[64];
    static u128 p10[21];
    if (!init) {
        p10[0] = 1;
        for (int i = 1; i <= 20; i++) p10[i] = p10[i - 1] * 10;
        for (int i = 0; i < 64; i++) {
            uint64_t v = (uint64_t)1 << i;
            unsigned c = 0;
            while (v) { c++; v /= 10; }
            digs[i] = c;
        }
        init = true;
    }
    int d = 63;
    while (d > 0 && !((m >> d) & 1)) d--;
    u128 r = 0;
    for (int i = d; i >= 0; i--) {
        if ((m >> i) & 1) {
            u128 mult = p10[digs[i]] % m;
            u128 val = ((u128)1 << i) % m;
            r = (r * mult + val) % m;
        } else {
            r = (r * 10) % m;
        }
    }
    return r == 0;
}

seq_t a(idx_t n)
{
    static uint64_t *found = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t last = 0;
    while (cnt < (size_t)n) {
        uint64_t m = ++last;
        if (A303492_check(m)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                found = (uint64_t *)realloc(found, cap * sizeof(uint64_t));
            }
            found[cnt++] = m;
        }
    }
    return (seq_t)found[n - 1];
}
