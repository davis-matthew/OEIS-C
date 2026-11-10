/*
 * A266154  Inverse permutation to A266089.
 * https://oeis.org/A266154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A266089(k) = n (inverse permutation); A266089
 *            is built by greedy search (smallest unused number whose popcount
 *            differs by 1 from the previous term's), starting with
 *            A266089(0)=0
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9329 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static unsigned long long *A266154_pos = NULL; /* value -> index+1, 0 = unused */
static size_t A266154_cap = 0;
static unsigned long long A266154_len = 0;     /* terms generated */
static unsigned long long A266154_lo = 0;      /* smallest unused value */
static unsigned long long A266154_prev = 0;

static unsigned A266154_pc(unsigned long long v)
{
    unsigned c = 0;
    while (v) { c += (unsigned)(v & 1); v >>= 1; }
    return c;
}

static void A266154_ensure(unsigned long long idx)
{
    if (idx < A266154_cap) return;
    size_t nc = A266154_cap ? A266154_cap : 1024;
    while (nc <= idx) nc <<= 1;
    unsigned long long *p = (unsigned long long *)realloc(A266154_pos, nc * sizeof(unsigned long long));
    if (!p) exit(EXIT_FAILURE);
    memset(p + A266154_cap, 0, (nc - A266154_cap) * sizeof(unsigned long long));
    A266154_pos = p;
    A266154_cap = nc;
}

seq_t a(idx_t n)
{
    unsigned long long target = (unsigned long long)n;
    if (A266154_len == 0) {
        A266154_ensure(0);
        A266154_pos[0] = 1;
        A266154_len = 1;
        A266154_prev = 0;
        A266154_lo = 1;
    }
    A266154_ensure(target);
    while (A266154_pos[target] == 0) {
        unsigned pp = A266154_pc(A266154_prev);
        unsigned long long c = A266154_lo;
        for (;;) {
            A266154_ensure(c);
            if (A266154_pos[c] == 0) {
                unsigned pc = A266154_pc(c);
                if (pc == pp + 1 || pc + 1 == pp) break;
            }
            ++c;
        }
        A266154_pos[c] = A266154_len + 1;
        ++A266154_len;
        A266154_prev = c;
        while (1) {
            A266154_ensure(A266154_lo);
            if (A266154_pos[A266154_lo] == 0) break;
            ++A266154_lo;
        }
    }
    return (seq_t)(A266154_pos[target] - 1);
}
