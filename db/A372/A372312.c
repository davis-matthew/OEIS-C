/*
 * A372312  Row sums of A372311.
 * https://oeis.org/A372312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n^k * Sum_{j=0..n} binomial(n-j, n-k) *
 *            Eulerian1(n, j), the row sums of A372311; Eulerian numbers by
 *            E(i,j) = (j+1)*E(i-1,j) + (i-j)*E(i-1,j-1), E(0,0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-10)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = n;
    seq_t *prev = mem_new(seq_t, (r + 2));
    seq_t *curr = mem_new(seq_t, (r + 2));
    seq_t *pw = mem_new(seq_t, (r + 2));
    prev[0] = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = 0; j <= i; ++j) {
            seq_t t1 = 0, t2 = 0;
            if (j <= i - 1) t1 = (seq_t)(j + 1) * prev[j];
            if (j >= 1) t2 = (seq_t)(i - j) * prev[j - 1];
            curr[j] = t1 + t2;
        }
        for (idx_t j = i + 1; j <= r; ++j) curr[j] = 0;
        seq_t *tmp = prev; prev = curr; curr = tmp;
    }
    pw[0] = (seq_t)1;
    for (idx_t k = 1; k <= r; ++k) pw[k] = pw[k - 1] * (seq_t)r;

    seq_t total = 0;
    for (idx_t j = 0; j <= r; ++j) {
        if (prev[j] == 0) continue;
        seq_t inner = 0;
        for (idx_t k = j; k <= r; ++k) {
            inner += pw[k] * binomial(r - j, r - k);
        }
        total += prev[j] * inner;
    }
    mem_free(prev);
    mem_free(curr);
    mem_free(pw);
    return total;
}
