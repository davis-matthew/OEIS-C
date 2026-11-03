/*
 * A355292  a(n) = Sum_{k=1..n} |Stirling1(n,k)| * Catalan(k-1).
 * https://oeis.org/A355292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} |Stirling1(n,k)| * Catalan(k-1) (offset 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355292: a(n) = Sum_{k=1..n} |s1(n,k)| Catalan(k-1), the Catalan numbers by C(j+1) = Sum C(i) C(j-i).
 * Stirling rows by their recurrences and plain products only, so exact modulo
 * the width (the original's caches read entries that realloc had left unset,
 * or divided values that had wrapped). */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    const size_t N = (size_t)n;
    seq_t *c = mem_new(seq_t, N + 1), *Cat = mem_new(seq_t, N + 1);
    if (!c || !Cat) abort();
    c[0] = Cat[0] = (seq_t)1;
    for (size_t j = 1; j <= N; ++j)
        for (size_t i = 0; i < j; ++i) Cat[j] += Cat[i] * Cat[j - 1 - i];
    for (size_t m = 1; m <= N; ++m) {
        for (size_t k = m; k >= 1; --k) c[k] = c[k - 1] + (seq_t)(m - 1) * c[k];
        c[0] = (seq_t)0;
    }
    seq_t sum = (seq_t)0;
    for (size_t k = 1; k <= N; ++k) sum += c[k] * Cat[k - 1];
    mem_free(c); mem_free(Cat);
    return sum;
}
