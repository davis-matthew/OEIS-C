/*
 * A362293  Expansion of e.g.f. exp( (-LambertW(-x^3))^(1/3) ).
 * https://oeis.org/A362293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/3)} A362292(k) * binomial(n-1,3*k)
 *            * a(n-3*k-1), a(0) = 1, with A362292(k) = (k+1/3)^k * (3k)!/k!
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 420 (2026-11-10)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

static seq_t A362292_a(idx_t n)
{
    idx_t start = n + 1;
    idx_t limit = 3 * n;
    seq_t result = 1;

    idx_t cnt3 = 0;
    {
        idx_t t = limit;
        while (t) { t /= 3; cnt3 += t; }
        t = n;
        while (t) { t /= 3; cnt3 -= t; }
    }

    for (idx_t k = start; k <= limit; ++k)
    {
        result *= (seq_t)k;
        while (cnt3 && (result % 3) == 0)
        {
            result /= 3;
            --cnt3;
        }
    }

    seq_t base  = (seq_t)(3 * n + 1);
    seq_t power = ipow(base, (seq_t)n);
    result *= power;

    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t K = (n - 1) / 3;
    seq_t *g   = mem_new(seq_t, K + 1);
    seq_t *av  = mem_new(seq_t, n + 1);
    seq_t *row = mem_new(seq_t, n + 1);

    for (idx_t k = 0; k <= K; ++k) g[k] = A362292_a(k);

    av[0] = 1;
    row[0] = 1; /* row holds binomial(m-1, j) at start of iteration m */

    for (idx_t m = 1; m <= n; ++m)
    {
        seq_t s = 0;
        idx_t km = (m - 1) / 3;
        for (idx_t k = 0; k <= km; ++k)
        {
            s += g[k] * row[3 * k] * av[m - 3 * k - 1];
        }
        av[m] = s;
        /* advance row to binomial(m, j) */
        for (idx_t j = m; j >= 1; --j)
            row[j] += row[j - 1];
    }

    seq_t res = av[n];
    mem_free(g);
    mem_free(av);
    mem_free(row);
    return res;
}
