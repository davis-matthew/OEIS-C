/*
 * A308049  Inverse permutation to A308021 (Recamán variant).
 * https://oeis.org/A308049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index i at which A308021(i) = n (n-th smallest term of
 *            A308021); computed by simulating A308021 until values 1..n have
 *            all appeared
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint64_t A308049_cap = 0;
static uint8_t *A308049_vis = NULL;
static uint8_t *A308049_used = NULL;
static uint64_t *A308049_pos = NULL;
static uint64_t A308049_cur = 1, A308049_minu = 1, A308049_have = 1;

static void A308049_ensure(uint64_t m)
{
    if (m < A308049_cap) return;
    uint64_t nc = A308049_cap ? A308049_cap : 1024;
    while (nc <= m) nc <<= 1;
    A308049_vis = (uint8_t *)realloc(A308049_vis, nc);
    A308049_used = (uint8_t *)realloc(A308049_used, nc);
    A308049_pos = (uint64_t *)realloc(A308049_pos, nc * sizeof(uint64_t));
    memset(A308049_vis + A308049_cap, 0, nc - A308049_cap);
    memset(A308049_used + A308049_cap, 0, nc - A308049_cap);
    memset(A308049_pos + A308049_cap, 0, (nc - A308049_cap) * sizeof(uint64_t));
    A308049_cap = nc;
}

static void A308049_step(void)
{
    A308049_ensure(A308049_cur);
    A308049_vis[A308049_cur] = 1;
    for (;;) {
        A308049_ensure(A308049_minu);
        if (!A308049_vis[A308049_minu]) break;
        A308049_minu++;
    }
    uint64_t m = A308049_minu, d;
    for (;;) {
        A308049_ensure(m);
        if (!A308049_vis[m]) {
            d = (m > A308049_cur) ? (m - A308049_cur) : (A308049_cur - m);
            A308049_ensure(d);
            if (!A308049_used[d]) break;
        }
        m++;
    }
    A308049_pos[d] = A308049_cur;
    A308049_used[d] = 1;
    A308049_vis[m] = 1;
    A308049_cur = m;
    for (;;) {
        A308049_ensure(A308049_have);
        if (!A308049_pos[A308049_have]) break;
        A308049_have++;
    }
}

seq_t a(idx_t n)
{
    uint64_t t = (uint64_t)n;
    A308049_ensure(t + 1);
    while (A308049_have <= t) A308049_step();
    return (seq_t)A308049_pos[t];
}
