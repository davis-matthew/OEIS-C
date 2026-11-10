/*
 * A258354  Smallest m such that A212306(m) = n.
 * https://oeis.org/A258354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m with A212306(m) = n; A212306(m) = m - sum of
 *            A212306(i), i<m, i+A212306(i) >= m (found by incremental scan)
 * Range:     u8 n <= 14, u16 n <= 485, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 953 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    size_t cap = 1024;
    seq_t *expire = mem_new(seq_t, cap);
    if (!expire) return (seq_t)0;
    seq_t S = 0;
    seq_t target = (seq_t)n;
    for (idx_t m = 1;; ++m) {
        if ((size_t)m < cap) {
            S -= expire[(size_t)m];
        }
        seq_t v = (seq_t)m - S;
        if (v == target) {
            mem_free(expire);
            return (seq_t)m;
        }
        if (v != 0) {
            size_t pos = (size_t)m + (size_t)v + 1;
            if (pos >= cap) {
                size_t ncap = cap;
                while (pos >= ncap) ncap *= 2;
                seq_t *ne = mem_new(seq_t, ncap);
                if (!ne) { mem_free(expire); return (seq_t)0; }
                for (size_t k = 0; k < cap; ++k) ne[k] = expire[k];
                mem_free(expire);
                expire = ne;
                cap = ncap;
            }
            S += v;
            expire[pos] += v;
        }
    }
}
