/*
 * A290715  Number of minimal edge covers in the n-barbell graph.
 * https://oeis.org/A290715
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A053530(n)^2 + A053530(n-1)*(A053530(n-1) + 2 +
 *            2*Sum_{i=1..n-2} binomial(n-1,i)*A053530(i))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 99 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Fills B[0..n] with A053530: B(0)=1, B(1)=0,
   B(m)=Sum_{s=1..m-1} C(m-1,s)*w(s+1)*B(m-s-1), w(2)=1, w(k)=k otherwise. */
static void A290715_fill(seq_t *B, idx_t n)
{
    B[0] = (seq_t)1;
    if (n >= 1) B[1] = (seq_t)0;
    for (idx_t m = 2; m <= n; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;
        for (idx_t s = 1; s <= m - 1; ++s) {
            binom = (binom * (seq_t)(m - s)) / (seq_t)s;
            seq_t w = (s == 1) ? (seq_t)1 : (seq_t)(s + 1);
            sum += binom * w * B[m - s - 1];
        }
        B[m] = sum;
    }
}

seq_t a(idx_t n)
{
    seq_t *B = mem_new(seq_t, (size_t)n + 2);
    A290715_fill(B, n);

    seq_t sum = 0;
    seq_t binom = 1; /* C(n-1,0) */
    for (idx_t i = 1; i + 2 <= n; ++i) {
        binom = (binom * (seq_t)(n - i)) / (seq_t)i;
        sum += binom * B[i];
    }
    seq_t bn = B[n];
    seq_t bm = B[n - 1];
    seq_t res = bn * bn + bm * (bm + 2 + 2 * sum);
    mem_free(B);
    return res;
}
