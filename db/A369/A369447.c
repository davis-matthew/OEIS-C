/*
 * A369447  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A001065(i) = A001065(j) and A342671(i) = A342671(j), for all i,
 *          j >= 1.
 * https://oeis.org/A369447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of the pairs [A001065(n),
 *            A342671(n)]; computed by labelling each n with the rank of the
 *            first occurrence of its pair
 * Range:     u8 n <= 75, u16 n <= 2741, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 108 terms through n = 9303 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A369447_isp(idx_t x) {
    if (x < 2) return false;
    for (idx_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static idx_t A369447_nextp(idx_t p) {
    idx_t q = p + 1;
    while (!A369447_isp(q)) q++;
    return q;
}

static void A369447_pair(idx_t n, seq_t *s1, seq_t *g) {
    seq_t sigma = 1, a3 = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t q = A369447_nextp(p);
            seq_t term = 1, pw = 1, qq = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                term += pw;
                qq *= (seq_t)q;
            }
            sigma *= term;
            a3 *= qq;
        }
    }
    if (m > 1) {
        sigma *= ((seq_t)m + 1);
        a3 *= (seq_t)A369447_nextp(m);
    }
    *s1 = sigma - (seq_t)n;
    seq_t x = sigma, y = a3;
    while (y != 0) {
        seq_t r = x % y;
        x = y;
        y = r;
    }
    *g = x;
}

static idx_t *A369447_lab = NULL;
static idx_t A369447_cap = 0;

seq_t a(idx_t n) {
    if (n > A369447_cap) {
        idx_t cap = A369447_cap * 2;
        if (cap < n) cap = n;
        if (cap < 64) cap = 64;
        if (A369447_lab) free(A369447_lab);
        A369447_lab = (idx_t *)malloc((size_t)(cap + 1) * sizeof(idx_t));
        seq_t *r1 = mem_new(seq_t, (size_t)cap + 1);
        seq_t *r2 = mem_new(seq_t, (size_t)cap + 1);
        idx_t distinct = 0;
        for (idx_t i = 1; i <= cap; i++) {
            seq_t x, y;
            A369447_pair(i, &x, &y);
            idx_t found = 0;
            for (idx_t k = 0; k < distinct; k++) {
                if (r1[k] == x && r2[k] == y) { found = k + 1; break; }
            }
            if (!found) {
                r1[distinct] = x;
                r2[distinct] = y;
                distinct++;
                found = distinct;
            }
            A369447_lab[i] = found;
        }
        mem_free(r1);
        mem_free(r2);
        A369447_cap = cap;
    }
    return (seq_t)A369447_lab[n];
}
