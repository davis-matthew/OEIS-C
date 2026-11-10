/*
 * A271861  Recursive sequence based on the central polygonal numbers
 *          (A000124) and A002260.
 * https://oeis.org/A271861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   start with b(i) = i. For k = 1, 2, 3, ...: let j = A002260(k)
 *            and x = b(j), then reverse the x terms b(k+1..k+x). a(n) = b(n)
 *            after steps 1..n-1 (position n is final by then)
 * Range:     u8 n <= 242, u16 n <= 1227, u32 n <= 1227, u64 n <= 1227,
 *            u128 n <= 1227, big any n
 * Validated: standard, pass, 83 terms through n = 1066 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *s_arr = 0;
static size_t s_cap = 0;
static uint64_t s_done = 0;   /* steps completed */
static uint64_t s_row = 1, s_col = 1;

static void ensure_cap(size_t need)
{
    if (need <= s_cap) return;
    size_t nc = s_cap ? s_cap : 1024;
    while (nc < need) nc *= 2;
    uint64_t *p = (uint64_t *)realloc(s_arr, nc * sizeof(uint64_t));
    if (!p) abort();
    for (size_t i = s_cap; i < nc; i++) p[i] = (uint64_t)i;
    s_arr = p;
    s_cap = nc;
}

seq_t a(idx_t n)
{
    ensure_cap((size_t)n + 2);
    while (s_done + 1 < n) {
        uint64_t k = s_done + 1;
        uint64_t x = s_arr[s_col];
        ensure_cap((size_t)(k + x + 2));
        x = s_arr[s_col];
        uint64_t lo = k + 1, hi = k + x;
        while (lo < hi) {
            uint64_t t = s_arr[lo];
            s_arr[lo] = s_arr[hi];
            s_arr[hi] = t;
            lo++;
            hi--;
        }
        s_done = k;
        s_col++;
        if (s_col > s_row) { s_row++; s_col = 1; }
    }
    return (seq_t)s_arr[n];
}
