/*
 * A361345  Numbers k such that A361338(k) = 6.
 * https://oeis.org/A361345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that A361338(k) = 6; computed by incremental
 *            search with a bottom-up table of reachable single-digit masks
 * Range:     u8 none, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static unsigned short *A361345_mask = NULL;
static size_t A361345_cap = 0;
static uint64_t *A361345_terms = NULL;
static size_t A361345_nterms = 0, A361345_tcap = 0;
static uint64_t A361345_next = 0; /* next k to examine */

static void A361345_grow(size_t need)
{
    if (need <= A361345_cap) return;
    size_t nc = A361345_cap ? A361345_cap : 1024;
    while (nc < need) nc <<= 1;
    unsigned short *p = (unsigned short *)realloc(A361345_mask, nc * sizeof(unsigned short));
    if (!p) exit(EXIT_FAILURE);
    A361345_mask = p;
    A361345_cap = nc;
}

static void A361345_compute(uint64_t v)
{
    unsigned short m = 0;
    if (v <= 9) {
        m = (unsigned short)(1u << (unsigned)v);
    } else {
        uint64_t div = 10;
        while (div <= v) {
            uint64_t left = v / div, right = v % div;
            /* right part has no leading zeros unless it is 0 (single digit) */
            if (!(div > 10 && right < div / 10)) {
                m |= A361345_mask[left * right];
            }
            div *= 10;
        }
    }
    A361345_mask[v] = m;
}

seq_t a(idx_t n)
{
    while (A361345_nterms < (size_t)n) {
        uint64_t k = A361345_next++;
        A361345_grow((size_t)k + 1);
        A361345_compute(k);
        unsigned m = A361345_mask[k];
        int c = 0;
        while (m) { c += (int)(m & 1u); m >>= 1; }
        if (c == 6) {
            if (A361345_nterms == A361345_tcap) {
                size_t nc = A361345_tcap ? A361345_tcap * 2 : 256;
                uint64_t *p = (uint64_t *)realloc(A361345_terms, nc * sizeof(uint64_t));
                if (!p) exit(EXIT_FAILURE);
                A361345_terms = p;
                A361345_tcap = nc;
            }
            A361345_terms[A361345_nterms++] = k;
        }
    }
    return (seq_t)A361345_terms[(size_t)n - 1];
}
