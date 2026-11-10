/*
 * A397326  a(n) = Sum_{j=0..n} (-1)^j*binomial(n, j) * A001006(2*n - j)^2.
 * https://oeis.org/A397326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} (-1)^j*binomial(n, j) * A001006(2*n - j)^2,
 *            with A001006 the Motzkin numbers: M(0)=M(1)=1, (k+2)*M(k) =
 *            (2k+1)*M(k-1) + (3k-3)*M(k-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-10)
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
    idx_t top = 2 * n;
    seq_t *M = mem_new(seq_t, (size_t)top + 2);
    M[0] = 1;
    M[1] = 1;
    for (idx_t i = 2; i <= top; ++i) {
        seq_t num = ((seq_t)2 * (seq_t)i + 1) * M[i - 1]
                  + ((seq_t)3 * (seq_t)i - 3) * M[i - 2];
        M[i] = num / ((seq_t)i + 2);
    }
    seq_t pos = 0, neg = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t m = M[top - j];
        seq_t term = binomial(n, j) * m * m;
        if (j & 1U) neg += term;
        else pos += term;
    }
    mem_free(M);
    return pos - neg;
}
