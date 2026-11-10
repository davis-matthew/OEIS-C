/*
 * A380843  Antidiagonal sums of the array A380841.
 * https://oeis.org/A380843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i+k=n} A(i,k), where A(i,k) = i! * Sum_{j=0..i}
 *            j^(i-j) * C(j+k-1,j)/(i-j)! (A380841), A(0,k)=1, A(i,0)=0 for
 *            i>0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-10)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

static seq_t A380843_pow(seq_t b, int e)
{
    seq_t r = (seq_t)1;
    while (e > 0) {
        if (e & 1) r = r * b;
        b = b * b;
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    idx_t s = n;
    idx_t w = s + 1u;
    seq_t *C = mem_new(seq_t, (size_t)(w * w));
    /* Pascal triangle: C[m*w + r] = binomial(m, r), additions only */
    for (idx_t m = 0u; m <= s; ++m) {
        C[m * w] = (seq_t)1;
        for (idx_t r = 1u; r <= m; ++r) {
            seq_t v = C[(m - 1u) * w + (r - 1u)];
            if (r <= m - 1u) v = v + C[(m - 1u) * w + r];
            C[m * w + r] = v;
        }
    }
    seq_t result = (seq_t)1; /* A(0,s) = 1 */
    for (idx_t i = 1u; i < s; ++i) {
        idx_t k = s - i;
        seq_t P = (seq_t)1;
        for (idx_t j = 1u; j <= i; ++j) {
            P = P * (seq_t)(i - j + 1u);   /* i!/(i-j)! */
            seq_t jp = A380843_pow((seq_t)j, (int)(i - j));
            seq_t B = C[(j + k - 1u) * w + j];
            result += P * jp * B;
        }
    }
    mem_free(C);
    return result;
}
