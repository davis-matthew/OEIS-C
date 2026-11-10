/*
 * A396198  a(n) = Sum_{k=0..n} (-1)^k*A396200(n, k), even bisection only.
 * https://oeis.org/A396198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..2n} (-1)^k * T(2n,k), where T(r,k) =
 *            Sum_{i=0..max(k,r-k)} binomial(r+1,i) (A396200 as validated)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = 2 * n;
    idx_t N = r + 1;
    seq_t *P = mem_new(seq_t, (size_t)N + 1);
    seq_t c = 1;
    seq_t run = 0;
    for (idx_t i = 0; i <= N; ++i) {
        run += c;
        P[i] = run;
        if (i < N) {
            c = c * (seq_t)(N - i) / (seq_t)(i + 1);
        }
    }
    seq_t pos = 0, neg = 0;
    for (idx_t k = 0; k <= r; ++k) {
        idx_t m = (k > r - k) ? k : r - k;
        if ((k & 1) == 0) pos += P[m];
        else neg += P[m];
    }
    mem_free(P);
    return pos - neg;
}
