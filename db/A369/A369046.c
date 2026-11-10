/*
 * A369046  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A003415(i) = A003415(j) and A069359(i) = A069359(j), for all i,
 *          j >= 1.
 * https://oeis.org/A369046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of the pairs [A003415(n),
 *            A069359(n)] (index of first occurrence of each pair, numbered in
 *            order of first appearance); computed by incremental scan with
 *            memoization
 * Range:     u8 n <= 367, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 107 terms through n = 9620 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static void A369046_pair(idx_t n, seq_t *der, seq_t *sf) {
    seq_t d = 0, s = 0;
    idx_t m = n;
    if (n > 1) {
        for (idx_t p = 2; p <= m / p; p++) {
            if (m % p == 0) {
                int e = 0;
                while (m % p == 0) { m /= p; e++; }
                d += (seq_t)(n / p) * (seq_t)e;
                s += (seq_t)(n / p);
            }
        }
        if (m > 1) {
            d += (seq_t)(n / m);
            s += (seq_t)(n / m);
        }
    }
    *der = d;
    *sf = s;
}

seq_t a(idx_t n) {
    static idx_t cap = 0, done = 0, ncls = 0;
    static seq_t *D = 0, *S = 0;
    static idx_t *rank = 0, *rep = 0;
    if (n < 1) return 0;
    if (n >= cap) {
        idx_t nc = cap ? cap : 1024;
        while (nc <= n) nc *= 2;
        if (cap == 0) {
            D = mem_new(seq_t, nc);
            S = mem_new(seq_t, nc);
            rank = mem_new(idx_t, nc);
            rep = mem_new(idx_t, nc);
        } else {
            D = mem_resize(seq_t, D, nc);
            S = mem_resize(seq_t, S, nc);
            rank = mem_resize(idx_t, rank, nc);
            rep = mem_resize(idx_t, rep, nc);
        }
        cap = nc;
    }
    while (done < n) {
        idx_t m = done + 1;
        A369046_pair(m, &D[m], &S[m]);
        idx_t found = 0;
        for (idx_t j = 0; j < ncls; j++) {
            idx_t r = rep[j];
            if (D[r] == D[m] && S[r] == S[m]) { found = j + 1; break; }
        }
        if (!found) {
            rep[ncls++] = m;
            found = ncls;
        }
        rank[m] = found;
        done = m;
    }
    return (seq_t)rank[n];
}
