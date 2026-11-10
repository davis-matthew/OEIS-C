/*
 * A361339  a(n) is the smallest k such that A361338(k) = n.
 * https://oeis.org/A361339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = smallest k >= 1 such that A361338(k) = n; found by a
 *            forward scan with a DP over reachable-digit bitmasks (split v =
 *            L|R, R without leading zeros unless single digit, then recurse
 *            on L*R < v)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint16_t *A361339_mask = NULL;
static size_t A361339_cap = 0;
static uint64_t A361339_scanned = 0;      /* values 0..scanned computed */
static uint64_t A361339_first[11];
static int A361339_have[11];
static int A361339_init = 0;

static void A361339_reserve(size_t need)
{
    if (need <= A361339_cap) return;
    size_t nc = A361339_cap ? A361339_cap : 1024;
    while (nc < need) nc <<= 1;
    uint16_t *p = (uint16_t *)realloc(A361339_mask, nc * sizeof(uint16_t));
    if (!p) exit(EXIT_FAILURE);
    A361339_mask = p;
    A361339_cap = nc;
}

seq_t a(idx_t n)
{
    if (!A361339_init) {
        for (int i = 0; i < 11; i++) { A361339_have[i] = 0; A361339_first[i] = 0; }
        A361339_reserve(1024);
        for (uint64_t v = 0; v <= 9; v++) A361339_mask[v] = (uint16_t)(1u << v);
        A361339_scanned = 9;
        /* k = 1..9 each reach exactly 1 digit */
        A361339_have[1] = 1;
        A361339_first[1] = 1;
        A361339_init = 1;
    }
    if (n < 1 || n > 10) return (seq_t)0;
    int t = (int)n;
    while (!A361339_have[t]) {
        uint64_t v = ++A361339_scanned;
        A361339_reserve((size_t)v + 1);
        uint64_t p10[20];
        p10[0] = 1;
        for (int i = 1; i < 20; i++) p10[i] = p10[i - 1] * 10ULL;
        int len = 1;
        { uint64_t x = v; while (x >= 10) { x /= 10; len++; } }
        uint16_t m = 0;
        for (int lenR = 1; lenR < len; lenR++) {
            uint64_t d = p10[lenR];
            uint64_t left = v / d, right = v % d;
            if (lenR > 1 && right < p10[lenR - 1]) continue;
            m |= A361339_mask[left * right];
        }
        A361339_mask[v] = m;
        int c = 0;
        for (uint16_t x = m; x; x >>= 1) c += x & 1;
        if (c >= 1 && c <= 10 && !A361339_have[c]) {
            A361339_have[c] = 1;
            A361339_first[c] = v;
        }
    }
    return (seq_t)A361339_first[t];
}
