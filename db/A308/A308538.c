/*
 * A308538  a(n) = exp(1) * Sum_{k>=0} (-1)^k*(k*(k + 1))^n/k!.
 * https://oeis.org/A308538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*A000587(2*n-k), where A000587
 *            is the Rao Uppuluri-Carpenter numbers (Aitken array: T(0,0)=1,
 *            T(m+1,0)=-T(m,m), T(m+1,k+1)=T(m+1,k)+T(m,k), A000587(m)=T(m,0))
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    idx_t m = 2 * n;
    seq_t *terms = mem_new(seq_t, (size_t)(m + 2));
    seq_t *row = mem_new(seq_t, (size_t)(m + 2));
    idx_t t, k;
    seq_t c, sum;

    terms[0] = 1;
    row[0] = 1;
    for (t = 0; t < m; t++) {
        seq_t carry = 0;
        seq_t old;
        carry -= row[t];
        for (k = 0; k <= t; k++) {
            old = row[k];
            row[k] = carry;
            carry += old;
        }
        row[t + 1] = carry;
        terms[t + 1] = row[0];
    }

    sum = 0;
    c = 1;
    for (k = 0; k <= n; k++) {
        sum += c * terms[m - k];
        if (k < n) {
            c = c * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    mem_free(terms);
    mem_free(row);
    return sum;
}
