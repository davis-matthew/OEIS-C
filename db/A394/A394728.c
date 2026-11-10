/*
 * A394728  Expansion of e.g.f. exp( sqrt(1-x^2) - 1 + x*arcsin(x) ) (even
 *          powers only).
 * https://oeis.org/A394728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} binomial(2*n-1,2*k-1) *
 *            A001818(k-1) * a(n-k), where A001818(j) = ((2*j-1)!!)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-10)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    seq_t *r = mem_new(seq_t, N + 1);
    seq_t *sq = mem_new(seq_t, N + 1);
    seq_t prod = 1;
    sq[0] = 1;
    for (size_t j = 1; j <= N; j++) {
        prod = prod * ((seq_t)j * 2 - 1);
        sq[j] = prod * prod;
    }
    r[0] = 1;
    for (size_t m = 1; m <= N; m++) {
        seq_t s = 0;
        for (size_t k = 1; k <= m; k++) {
            seq_t c = binomial((idx_t)(2 * m - 1), (idx_t)(2 * k - 1));
            s += c * sq[k - 1] * r[m - k];
        }
        r[m] = s;
    }
    seq_t res = r[N];
    mem_free(r);
    mem_free(sq);
    return res;
}
