/*
 * A361342  Numbers k such that A361338(k) = 3.
 * https://oeis.org/A361342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 0 such that A361338(k) = 3, found by scanning k
 *            upward with a bottom-up DP on reachable single-digit masks
 *            (split-and-multiply)
 * Range:     u8 n <= 19, u16 n <= 765, u32 n <= 775, u64 n <= 775,
 *            u128 n <= 775, big any n
 * Validated: standard, pass, 87 terms through n = 673 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned *A361342_mask = NULL;
static size_t A361342_cap = 0;
static uint64_t A361342_next = 0;
static uint64_t *A361342_terms = NULL;
static size_t A361342_tcnt = 0, A361342_tcap = 0;

static void A361342_compute(uint64_t v)
{
    if (v + 1 > A361342_cap) {
        size_t nc = A361342_cap ? A361342_cap : 1024;
        while (nc < v + 1) nc <<= 1;
        unsigned *p = (unsigned *)realloc(A361342_mask, nc * sizeof(unsigned));
        if (!p) exit(1);
        A361342_mask = p;
        A361342_cap = nc;
    }
    if (v <= 9) {
        A361342_mask[v] = 1u << (unsigned)v;
        return;
    }
    unsigned m = 0;
    uint64_t pw = 10;
    while (v / pw >= 1) {
        uint64_t left = v / pw, right = v % pw;
        /* right part has digit length lenR where pw = 10^lenR */
        if (!(pw > 10 && right < pw / 10)) {
            uint64_t prod = left * right;
            m |= A361342_mask[prod];
        }
        pw *= 10;
    }
    A361342_mask[v] = m;
}

seq_t a(idx_t n)
{
    while (A361342_tcnt < (size_t)n) {
        uint64_t v = A361342_next++;
        A361342_compute(v);
        unsigned m = A361342_mask[v];
        int c = 0;
        while (m) { c += m & 1u; m >>= 1; }
        if (c == 3) {
            if (A361342_tcnt == A361342_tcap) {
                size_t nc = A361342_tcap ? A361342_tcap * 2 : 256;
                uint64_t *p = (uint64_t *)realloc(A361342_terms, nc * sizeof(uint64_t));
                if (!p) exit(1);
                A361342_terms = p;
                A361342_tcap = nc;
            }
            A361342_terms[A361342_tcnt++] = v;
        }
    }
    return (seq_t)A361342_terms[(size_t)n - 1];
}
