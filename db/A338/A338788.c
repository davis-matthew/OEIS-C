/*
 * A338788  a(n) = (n!/6) * Sum_{i,j,k > 0 and i+j+k=n}
 *          A000593(i)*A000593(j)*A000593(k)/(i*j*k).
 * https://oeis.org/A338788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!/6) * Sum_{i+j+k=n, i,j,k>0}
 *            A000593(i)*A000593(j)*A000593(k)/(i*j*k); computed as c3(n)/6
 *            where b(i)=A000593(i)*(i-1)!, c2=binomial convolution of b with
 *            b, c3=binomial convolution of b with c2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 356 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t A000593_a(idx_t n) {
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
    }
    seq_t result = 0;
    for (idx_t d = 1; d <= m; ++d) {
        if (m % d == 0) {
            result += (seq_t)d;
            idx_t complement = m / d;
            if (d < complement) {
                result += (seq_t)complement;
            }
        }
        if (d > m / d) break;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;
    size_t N = (size_t)n;
    size_t W = N + 1;
    seq_t *C = mem_new(seq_t, W * W);
    seq_t *b = mem_new(seq_t, W);
    seq_t *c2 = mem_new(seq_t, W);
    for (size_t r = 0; r <= N; r++) {
        C[r * W] = (seq_t)1;
        for (size_t k = 1; k <= r; k++) {
            seq_t v = C[(r - 1) * W + (k - 1)];
            if (k <= r - 1) v += C[(r - 1) * W + k];
            C[r * W + k] = v;
        }
    }
    seq_t fact = (seq_t)1; /* (i-1)! */
    for (size_t i = 1; i <= N; i++) {
        if (i > 1) fact = fact * (seq_t)(i - 1);
        b[i] = A000593_a((idx_t)i) * fact;
    }
    for (size_t m2 = 2; m2 < N; m2++) {
        seq_t s = 0;
        for (size_t i = 1; i < m2; i++) {
            s += C[m2 * W + i] * b[i] * b[m2 - i];
        }
        c2[m2] = s;
    }
    seq_t s3 = 0;
    for (size_t i = 1; i + 2 <= N; i++) {
        s3 += C[N * W + i] * b[i] * c2[N - i];
    }
    s3 = s3 / 6;
    mem_free(C);
    mem_free(b);
    mem_free(c2);
    return s3;
}
