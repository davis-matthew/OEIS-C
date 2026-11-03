/*
 * A377448  E.g.f. satisfies A(x) = 1/(1 + A(x)^4 * log(1 - x)).
 * https://oeis.org/A377448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 0..n} (5*k)!/(4*k+1)! * |Stirling1(n,k)|
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A377448: a(n) = Sum_{k=0..n} (5k)!/(4k+1)! |s1(n,k)|, the ratio being the product (4k+2)(4k+3)...(5k). The Stirling row |s1(n, .)| is built
 * for each n by its recurrence and the weights by plain products: ring
 * operations only, so exact modulo the width (the original's caches read
 * entries that realloc had left unset). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t N = (size_t)n;
    seq_t *s = mem_new(seq_t, N + 1);
    if (!s) abort();
    s[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = m; k >= 1; --k) s[k] = s[k - 1] + (seq_t)(m - 1) * s[k];
        s[0] = (seq_t)0;
    }
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k <= N; ++k) {
        seq_t w = (seq_t)1;
        for (size_t i = 4 * k + 2; i <= 5 * k; ++i) w *= (seq_t)i;
        sum += w * s[k];
    }
    mem_free(s);
    return sum;
}
