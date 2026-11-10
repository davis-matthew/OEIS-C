/*
 * A361344  Numbers k such that A361338(k) = 5.
 * https://oeis.org/A361344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that A361338(k) = 5; computed by a sieve-like
 *            scan, with the reachable single-digit mask of each k built from
 *            the masks of smaller values left*right over all digit splits
 * Range:     u8 none, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static unsigned short *A361344_mask = NULL;
static size_t A361344_cap = 0;
static uint64_t A361344_done = 0;      /* values 0..done-1 have masks */
static uint64_t *A361344_terms = NULL;
static size_t A361344_tcnt = 0, A361344_tcap = 0;

static void A361344_ensure(uint64_t v)
{
    if (v < A361344_cap) return;
    size_t nc = A361344_cap ? A361344_cap : 1024;
    while (nc <= v) nc <<= 1;
    unsigned short *p = (unsigned short *)realloc(A361344_mask, nc * sizeof(unsigned short));
    if (!p) exit(EXIT_FAILURE);
    A361344_mask = p;
    A361344_cap = nc;
}

static void A361344_extend(void)
{
    uint64_t v = A361344_done;
    A361344_ensure(v);
    unsigned m = 0;
    if (v <= 9) {
        m = 1u << (unsigned)v;
    } else {
        uint64_t div = 10, p10 = 1;
        while (div <= v) {
            /* right part has lenR digits, div = 10^lenR, p10 = 10^(lenR-1) */
            uint64_t left = v / div, right = v % div;
            if (left > 0 && !(div > 10 && right < p10)) {
                m |= A361344_mask[left * right];
            }
            p10 = div;
            div *= 10;
        }
    }
    A361344_mask[v] = (unsigned short)m;
    int c = 0;
    while (m) { c += (int)(m & 1u); m >>= 1; }
    if (v >= 1 && c == 5) {
        if (A361344_tcnt == A361344_tcap) {
            size_t nc = A361344_tcap ? A361344_tcap * 2 : 256;
            uint64_t *t = (uint64_t *)realloc(A361344_terms, nc * sizeof(uint64_t));
            if (!t) exit(EXIT_FAILURE);
            A361344_terms = t;
            A361344_tcap = nc;
        }
        A361344_terms[A361344_tcnt++] = v;
    }
    A361344_done++;
}

seq_t a(idx_t n)
{
    while (A361344_tcnt < (size_t)n)
        A361344_extend();
    return (seq_t)A361344_terms[n - 1];
}
