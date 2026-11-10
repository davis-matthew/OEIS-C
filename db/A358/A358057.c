/*
 * A358057  Inverse permutation to A357961.
 * https://oeis.org/A358057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that A357961(k) = n (inverse permutation of
 *            A357961), where A357961(1) = 1 and A357961(m+1) is the h-th
 *            positive integer not yet in the sequence, h = Hamming weight of
 *            A357961(m). Computed by simulating the sequence until n appears
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 8491 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint32_t *A358057_pos = NULL;   /* pos[v] = index of value v, 0 if unused */
static uint64_t A358057_cap = 0;
static uint64_t A358057_count = 0;
static uint64_t A358057_cur = 0;
static uint64_t A358057_low = 1;

static void A358057_ensure(uint64_t needed)
{
    if (needed <= A358057_cap)
        return;
    uint64_t nc = A358057_cap ? A358057_cap : 64;
    while (nc < needed)
        nc *= 2;
    uint32_t *t = (uint32_t *)realloc(A358057_pos, nc * sizeof(uint32_t));
    memset(t + A358057_cap, 0, (nc - A358057_cap) * sizeof(uint32_t));
    A358057_pos = t;
    A358057_cap = nc;
}

static void A358057_step(void)
{
    uint64_t v;
    if (A358057_count == 0) {
        v = 1;
    } else {
        unsigned k = 0;
        uint64_t x = A358057_cur;
        while (x) { k += (unsigned)(x & 1); x >>= 1; }
        uint64_t p = A358057_low;
        unsigned cnt = 0;
        for (;;) {
            A358057_ensure(p + 1);
            if (A358057_pos[p] == 0) {
                ++cnt;
                if (cnt == k) break;
            }
            ++p;
        }
        v = p;
    }
    A358057_ensure(v + 1);
    A358057_count++;
    A358057_pos[v] = (uint32_t)A358057_count;
    A358057_cur = v;
    while (A358057_pos[A358057_low] != 0) {
        A358057_low++;
        A358057_ensure(A358057_low + 1);
    }
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    A358057_ensure(m + 1);
    while (A358057_pos[m] == 0) {
        A358057_step();
        A358057_ensure(m + 1);
    }
    return (seq_t)A358057_pos[m];
}
