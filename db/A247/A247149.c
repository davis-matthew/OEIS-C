/*
 * A247149  In decimal representation: digits of concatenation of A247143.
 * https://oeis.org/A247149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th digit (offset 0) of the concatenation of A247143;
 *            computed by simulating A247143 (greedy search)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 7813 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static bool A247149_init = false;
static unsigned char *A247149_dig = NULL;
static uint64_t A247149_dlen = 0, A247149_dcap = 0;
static bool *A247149_used = NULL;
static uint64_t A247149_ucap = 0;
static uint64_t A247149_steps = 0;
static uint64_t A247149_next_any = 0;
static uint64_t A247149_next_dig[10];

static bool A247149_has(uint64_t v, unsigned d)
{
    if (v == 0) return d == 0;
    while (v > 0) {
        if (v % 10 == d) return true;
        v /= 10;
    }
    return false;
}

static void A247149_ensure_used(uint64_t upto)
{
    if (A247149_ucap > upto) return;
    uint64_t nc = A247149_ucap == 0 ? 1024 : A247149_ucap;
    while (nc <= upto) nc <<= 1;
    bool *p = (bool *)realloc(A247149_used, nc * sizeof(bool));
    if (!p) exit(1);
    memset(p + A247149_ucap, 0, (nc - A247149_ucap) * sizeof(bool));
    A247149_used = p;
    A247149_ucap = nc;
}

static void A247149_push(unsigned char d)
{
    if (A247149_dlen == A247149_dcap) {
        uint64_t nc = A247149_dcap == 0 ? 1024 : A247149_dcap * 2;
        unsigned char *p = (unsigned char *)realloc(A247149_dig, nc);
        if (!p) exit(1);
        A247149_dig = p;
        A247149_dcap = nc;
    }
    A247149_dig[A247149_dlen++] = d;
}

static void A247149_step(void)
{
    uint64_t chosen;
    if (A247149_steps >= A247149_dlen) {
        uint64_t c = A247149_next_any;
        for (;;) {
            A247149_ensure_used(c);
            if (!A247149_used[c]) break;
            ++c;
        }
        chosen = c;
        A247149_next_any = c + 1;
    } else {
        unsigned r = A247149_dig[A247149_steps];
        uint64_t c = A247149_next_dig[r];
        for (;;) {
            A247149_ensure_used(c);
            if (!A247149_used[c] && A247149_has(c, r)) break;
            ++c;
        }
        chosen = c;
        A247149_next_dig[r] = c + 1;
    }
    A247149_ensure_used(chosen);
    A247149_used[chosen] = true;
    A247149_steps++;

    unsigned char buf[24];
    int p = 0;
    uint64_t t = chosen;
    if (t == 0) buf[p++] = 0;
    while (t > 0) {
        buf[p++] = (unsigned char)(t % 10);
        t /= 10;
    }
    for (int i = p - 1; i >= 0; --i) A247149_push(buf[i]);
}

seq_t a(idx_t n)
{
    if (!A247149_init) {
        for (int i = 0; i < 10; ++i) A247149_next_dig[i] = 0;
        A247149_init = true;
    }
    while (A247149_dlen <= (uint64_t)n) A247149_step();
    return (seq_t)A247149_dig[n];
}
