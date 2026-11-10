/*
 * A350790  Number of digraphs on n labeled nodes with a global source and
 *          sink.
 * https://oeis.org/A350790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(n-1)*A003030(n-1) for n >= 3; a(1) = 1, a(2) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 48 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A350790_grow(seq_t *old, idx_t oldlen, idx_t newlen)
{
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    idx_t i;
    for (i = 0; i < newlen; i++) p[i] = 0;
    for (i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static seq_t a3030_two_pow(idx_t e) {
    seq_t r = 1;
    seq_t b = 2;
    while (e) {
        if (e & 1) r = r * b;
        e >>= 1;
        if (e) b = b * b;
    }
    return r;
}

static seq_t *a3030_c = NULL;
static seq_t *a3030_s = NULL;
static seq_t *a3030_row = NULL;
static idx_t a3030_done = 0;
static idx_t a3030_cap = 0;

static void a3030_ensure(idx_t n) {
    if (n + 2 <= a3030_cap) return;
    idx_t nc = a3030_cap ? a3030_cap : 16;
    while (nc < n + 2) nc *= 2;
    a3030_c = A350790_grow(a3030_c, a3030_cap, nc);
    a3030_s = A350790_grow(a3030_s, a3030_cap, nc);
    a3030_row = A350790_grow(a3030_row, a3030_cap, nc);
    if (a3030_cap == 0) {
        a3030_row[0] = 1;
        a3030_c[0] = 0;
        a3030_s[0] = 0;
    }
    a3030_cap = nc;
}

static seq_t A003030_a(idx_t n) {
    if (n == 0) return 0;
    a3030_ensure(n);
    while (a3030_done < n) {
        idx_t m = a3030_done + 1;
        idx_t k;
        seq_t sum_s = 0;
        for (k = 1; k < m; k++)
            sum_s += a3030_row[k - 1] * a3030_s[k] * a3030_c[m - k];
        a3030_row[m] = 1;
        for (k = m - 1; k >= 1; k--)
            a3030_row[k] += a3030_row[k - 1];
        seq_t sub = 0;
        for (k = 1; k < m; k++) {
            idx_t r = m - k;
            sub += a3030_row[k] * a3030_c[k] * a3030_two_pow(k * r + r * (r - 1));
        }
        seq_t cm = a3030_two_pow(m * (m - 1)) - sub;
        a3030_c[m] = cm;
        a3030_s[m] = cm + sum_s;
        a3030_done = m;
    }
    return a3030_s[n];
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    if (n == 2) return 2;
    seq_t r = (seq_t)2 * (seq_t)n * ((seq_t)n - 1);
    return r * A003030_a(n - 1);
}
