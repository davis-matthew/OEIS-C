/*
 * A351175  Natural numbers k such that there are infinitely many
 *          representations k = A/B with A, B in A035928.
 * https://oeis.org/A351175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that k = A/B has infinitely many solutions
 *            with A, B in A035928; computed by a two-ended DFS search for
 *            representations at large lengths (heuristic test for infinitely
 *            many)
 * Range:     u8 n <= 38, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 u128_a351175;

static u128_a351175 g_k;
static int g_m, g_mp;

static bool a351175_inset(u128_a351175 A)
{
    if (A == 0) return false;
    int L = 0;
    u128_a351175 t = A;
    while (t) { L++; t >>= 1; }
    if (L & 1) return false;
    for (int i = 0; i < L / 2; i++) {
        int x = (int)((A >> i) & 1), y = (int)((A >> (L - 1 - i)) & 1);
        if (x == y) return false;
    }
    return true;
}

static bool a351175_dfs(int j, u128_a351175 t)
{
    int m = g_m, mp = g_mp;
    u128_a351175 low = 0;
    for (int i = 0; i < j; i++) {
        int tb = (int)((t >> (j - 1 - i)) & 1);
        if (!tb) low |= (u128_a351175)1 << i;
    }
    u128_a351175 Alow = (g_k * low) & (((u128_a351175)1 << j) - 1);
    u128_a351175 TA = 0;
    for (int i = 0; i < j; i++) {
        if (!((Alow >> i) & 1)) TA |= (u128_a351175)1 << (j - 1 - i);
    }
    u128_a351175 base = t << (2 * m - j);
    u128_a351175 lo = base | low;
    u128_a351175 hi = base | (((u128_a351175)1 << (2 * m - j)) - ((u128_a351175)1 << j)) | low;
    u128_a351175 cellLo = TA << (2 * mp - j);
    u128_a351175 cellHi = (TA + 1) << (2 * mp - j);
    if (g_k * hi < cellLo) return false;
    if (g_k * lo >= cellHi) return false;
    if (j == m) {
        return a351175_inset(g_k * lo);
    }
    if (a351175_dfs(j + 1, t * 2)) return true;
    return a351175_dfs(j + 1, t * 2 + 1);
}

static bool a351175_test(uint64_t k)
{
    g_k = k;
    for (int m = 30; m <= 50; m++) {
        for (int mp = m; mp <= m + 14; mp++) {
            g_m = m; g_mp = mp;
            if (a351175_dfs(1, 1)) return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_k = 1;
    if (n == 0) return 0;
    while (cnt < (size_t)n) {
        uint64_t k = next_k++;
        if (a351175_test(k)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 64;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = k;
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
