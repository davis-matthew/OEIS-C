/*
 * A356575  Expansion of e.g.f. ( Product_{k>0} 1/(1-x^k)^(1/k!) )^x.
 * https://oeis.org/A356575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} k*A087906(k-1)*binomial(n-1,k-1)*a(n-k),
 *            a(0)=1, a(1)=0; A087906(m) = Sum_{d|m} (m-1)!/(d-1)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* A087906(m) = sum over d|m of prod_{j=d}^{m-1} j, built by Horner-style descent. */
static seq_t A087906_val(idx_t m)
{
    seq_t sum = 0;
    seq_t p = 1;
    for (idx_t d = m; d >= 1; --d) {
        if (m % d == 0)
            sum += p;
        if (d > 1)
            p *= (seq_t)(d - 1);
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    size_t N = (size_t)n;
    seq_t *av  = mem_new(seq_t, N + 2);
    seq_t *g   = mem_new(seq_t, N + 2);
    seq_t *row = mem_new(seq_t, N + 2);
    for (size_t k = 2; k <= N; ++k)
        g[k] = (seq_t)k * A087906_val((idx_t)(k - 1));
    av[0] = 1;
    row[0] = 1; /* C(0, .) */
    for (size_t m = 1; m <= N; ++m) {
        seq_t s = 0;
        for (size_t k = 2; k <= m; ++k)
            s += g[k] * row[k - 1] * av[m - k];
        av[m] = s;
        /* update row to C(m, .) */
        for (size_t j = m; j >= 1; --j)
            row[j] += row[j - 1];
    }
    seq_t res = av[N];
    mem_free(av);
    mem_free(g);
    mem_free(row);
    return res;
}
