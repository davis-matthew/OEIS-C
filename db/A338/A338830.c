/*
 * A338830  a(n) is the greatest number not yet in the sequence with the same
 *          number of digits and the same largest digit as n.
 * https://oeis.org/A338830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 84 terms through n = 8529 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A338830: a(n) is the greatest number not yet in the sequence with the same
 * number of digits and the same largest digit as n. Generated in order: for
 * each class (digit count, largest digit) a pointer walks down the class's
 * members, skipping those already used; the terms are cached. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static size_t term_len = 0, term_cap = 0;
static unsigned char *used = NULL;        /* used[v] for v < 10^D */
static uint64_t ulim = 0;
static int64_t ptr[20][10];               /* next candidate per class, -1 if unset */

static int ndig(uint64_t v) { int d = 1; while (v >= 10) { v /= 10; ++d; } return d; }
static int maxdig(uint64_t v) { int m = 0; do { if ((int)(v % 10) > m) m = v % 10; v /= 10; } while (v); return m; }

seq_t a(idx_t n)
{
    if (!used) for (int i = 0; i < 20; ++i) for (int j = 0; j < 10; ++j) ptr[i][j] = -1;
    while (term_len <= (size_t)n) {
        uint64_t m = term_len;            /* compute a(m) */
        int d = ndig(m), x = maxdig(m);
        uint64_t top = 1;
        for (int i = 0; i < d; ++i) top *= 10;
        if (top > ulim) {
            unsigned char *u = (unsigned char *)realloc(used, (size_t)top);
            if (!u) abort();
            for (uint64_t i = ulim; i < top; ++i) u[i] = 0;
            used = u;
            ulim = top;
        }
        if (ptr[d][x] < 0) ptr[d][x] = (int64_t)top - 1;
        int64_t v = ptr[d][x];
        uint64_t lo = d == 1 ? 0 : top / 10;
        while (v >= (int64_t)lo && (used[v] || maxdig((uint64_t)v) != x)) --v;
        ptr[d][x] = v;
        used[v] = 1;
        if (term_len == term_cap) {
            term_cap = term_cap ? 2 * term_cap : 1024;
            uint64_t *t = (uint64_t *)realloc(terms, term_cap * sizeof *t);
            if (!t) abort();
            terms = t;
        }
        terms[term_len++] = (uint64_t)v;
    }
    return (seq_t)terms[n];
}
