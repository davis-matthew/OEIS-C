/*
 * A356566  Expansion of e.g.f. ( Product_{k>0} (1+x^k)^k )^x.
 * https://oeis.org/A356566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 0; a(n) = Sum_{k=2..n} k! * A078306(k-1)/(k-1)
 *            * binomial(n-1,k-1) * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

/* A078306(m) = Sum_{d|m} (-1)^(m/d+1) * d^2, computed by direct divisor sum. */
static seq_t A078306_a(idx_t m)
{
    seq_t s = 0;
    for (idx_t d = 1; d <= m; d++)
    {
        if (m % d == 0)
        {
            seq_t t = (seq_t)d * (seq_t)d;
            if (((m / d) & 1U) == 1U)
                s += t;
            else
                s -= t;
        }
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 0;
    idx_t N = n;
    seq_t *c = mem_new(seq_t, N + 1);
    seq_t *row = mem_new(seq_t, N + 1);
    seq_t *av = mem_new(seq_t, N + 1);

    /* c[k] = k! / (k-1) * A078306(k-1) = k * (k-2)! * A078306(k-1) */
    seq_t fact = 1; /* (k-2)! */
    for (idx_t k = 2; k <= N; k++)
    {
        if (k > 2)
            fact *= (seq_t)(k - 2);
        c[k] = (seq_t)k * fact * A078306_a(k - 1);
    }

    av[0] = 1;
    av[1] = 0;
    row[0] = 1; /* C(0, .) */
    for (idx_t m = 2; m <= N; m++)
    {
        /* update row from C(m-2, .) to C(m-1, .) */
        row[m - 1] = 0;
        for (idx_t j = m - 1; j >= 1; j--)
            row[j] += row[j - 1];
        seq_t s = 0;
        for (idx_t k = 2; k <= m; k++)
            s += c[k] * row[k - 1] * av[m - k];
        av[m] = s;
    }
    seq_t res = av[N];
    mem_free(c);
    mem_free(row);
    mem_free(av);
    return res;
}
