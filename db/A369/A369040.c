/*
 * A369040  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A003415(i) = A003415(j) and A342014(i) = A342014(j), for all i,
 *          j >= 1.
 * https://oeis.org/A369040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of the pair
 *            [A003415(n), A342014(n)]; computed by scanning n = 1.. and
 *            numbering distinct pairs in order of first appearance (memoized)
 * Range:     u8 n <= 391, u16 n <= 99999, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 107 terms through n = 9723 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A369040_deriv(seq_t n) {
    if (n <= 1) return 0;
    seq_t res = 0, rem = n;
    for (seq_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            seq_t e = 0;
            while (rem % p == 0) { e++; rem /= p; }
            res += e * (n / p);
        }
    }
    if (rem > 1) res += n / rem;
    return res;
}

static uint64_t *A369040_ids = NULL;
static seq_t *A369040_dd = NULL;
static seq_t *A369040_rr = NULL;
static uint64_t A369040_cap = 0;
static uint64_t A369040_done = 0;
static uint64_t A369040_nd = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n > A369040_done) {
        if (n > A369040_cap) {
            uint64_t nc = A369040_cap * 2;
            if (nc < (uint64_t)n) nc = (uint64_t)n;
            if (nc < 1024) nc = 1024;
            A369040_ids = (uint64_t *)realloc(A369040_ids, nc * sizeof(uint64_t));
            if (A369040_cap == 0) {
                A369040_dd = mem_new(seq_t, nc);
                A369040_rr = mem_new(seq_t, nc);
            } else {
                A369040_dd = mem_resize(seq_t, A369040_dd, nc);
                A369040_rr = mem_resize(seq_t, A369040_rr, nc);
            }
            A369040_cap = nc;
        }
        for (uint64_t m = A369040_done + 1; m <= (uint64_t)n; m++) {
            seq_t sm = (seq_t)m;
            seq_t d = A369040_deriv(sm);
            seq_t r = d % sm;
            uint64_t found = 0;
            for (uint64_t k = 0; k < A369040_nd; k++) {
                if (A369040_dd[k] == d && A369040_rr[k] == r) { found = k + 1; break; }
            }
            if (!found) {
                A369040_dd[A369040_nd] = d;
                A369040_rr[A369040_nd] = r;
                A369040_nd++;
                found = A369040_nd;
            }
            A369040_ids[m - 1] = found;
        }
        A369040_done = (uint64_t)n;
    }
    return (seq_t)A369040_ids[(uint64_t)n - 1];
}
