/*
 * A358668  a(n) is the least m such that A359194^k(m) = n for some k >= 0
 *          (where A359194^k denotes the k-th iterate of A359194).
 * https://oeis.org/A358668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least m such that A359194^k(m) = n for some k >= 0;
 *            computed by labelling forward orbits of m = 0,1,2,... (with
 *            A359194(m) = binary complement of 3m)
 * Range:     u8 n <= 31, u16 n <= 854, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8250 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t A359194_a(idx_t n)
{
    seq_t m = ((seq_t)n) * 3;
    size_t len = 0;
    seq_t tmp = m;
    while (tmp) {
        tmp >>= 1;
        ++len;
    }
    if (len == 0)
        len = 1;
    size_t bits = sizeof(seq_t) * 8;
    seq_t mask;
    if (len == bits) {
        mask = (seq_t)-1;
    } else {
        mask = (((seq_t)1) << len) - 1;
    }
    return mask - m;
}

static idx_t *a358668_label = NULL;
static idx_t a358668_N = 0;      /* labels valid for indices 0..a358668_N */
static idx_t a358668_cap = 0;

static void a358668_build(idx_t N)
{
    idx_t cap = 4 * N + 64;
    idx_t i, m;
    if (a358668_label) free(a358668_label);
    a358668_label = (idx_t *)malloc((size_t)(cap + 1) * sizeof(idx_t));
    for (i = 0; i <= cap; i++) a358668_label[i] = (idx_t)~(idx_t)0;
    for (m = 0; m <= N; m++) {
        seq_t x = (seq_t)m;
        int steps;
        for (steps = 0; steps < 400; steps++) {
            if (x <= (seq_t)cap) {
                idx_t xi = (idx_t)(unsigned long long)x;
                if (a358668_label[xi] != (idx_t)~(idx_t)0) break;
                a358668_label[xi] = m;
            }
            x = A359194_a((idx_t)x);
        }
    }
    a358668_N = N;
    a358668_cap = cap;
}

seq_t a(idx_t n)
{
    if (!a358668_label || n > a358668_N) {
        idx_t N = n < 1024 ? 1024 : n;
        if (a358668_label && N < 2 * a358668_N) N = 2 * a358668_N;
        a358668_build(N);
    }
    idx_t r = a358668_label[n];
    if (r == (idx_t)~(idx_t)0) r = n;
    return (seq_t)r;
}
