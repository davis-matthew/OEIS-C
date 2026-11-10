/*
 * A244226  Length of runs in A244221 (Greedy Catalan Base, A014418, reduced
 *          modulo 2).
 * https://oeis.org/A244226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th run (n >= 0) of equal values in
 *            A244221, where A244221(m) = A014418(m) mod 2 (parity found by
 *            greedy subtraction of Catalan numbers). Computed by scanning the
 *            parity sequence and counting runs
 * Range:     u8 n <= 34, u16 n <= 4120, u32 n <= 4120, u64 n <= 4120,
 *            u128 n <= 4120, big any n
 * Validated: standard, pass, 137 terms through n = 4077 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A244226_par(idx_t n)
{
    static seq_t *cat = NULL;
    static idx_t cat_len = 0;
    static idx_t cat_cap = 0;

    if (!cat) {
        cat_cap = 16;
        cat = mem_new(seq_t, cat_cap);
        cat[0] = (seq_t)1;
        cat_len = 1;
    }

    seq_t n_seq = (seq_t)n;
    while (cat_len > 0 && cat[cat_len - 1] <= n_seq) {
        if (cat_len >= cat_cap) {
            cat_cap <<= 1;
            cat = mem_resize(seq_t, cat, cat_cap);
        }
        idx_t k = cat_len - 1;
        seq_t prev = cat[k];
        seq_t num = prev * (seq_t)(2 * (2 * k + 1));
        seq_t denom = (seq_t)(k + 2);
        seq_t next = num / denom;
        cat[cat_len++] = next;
    }

    seq_t r = n_seq;
    while (r >= (seq_t)2) {
        idx_t lo = 0, hi = cat_len - 1, pos = 0;
        while (lo <= hi) {
            idx_t mid = (lo + hi) >> 1;
            if (cat[mid] <= r) {
                pos = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        r -= cat[pos];
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t m = 0;
    seq_t cur = A244226_par(0);
    for (idx_t k = 0; ; k++) {
        idx_t len = 1;
        while (A244226_par(m + len) == cur)
            len++;
        if (k == n)
            return (seq_t)len;
        m += len;
        cur = A244226_par(m);
    }
}
