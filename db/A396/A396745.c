/*
 * A396745  Expansion of e.g.f. -W_2(-x)/(1 - W_2(-x)), where W_k(x) is the
 *          k-th iterate of LambertW(x).
 * https://oeis.org/A396745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} n^(n-k) * binomial(n-1,k-1) * A277458(k),
 *            with A277458(1)=1, A277458(m) = m! * Sum_{k=1..m} (-1)^(k+1) * k
 *            * m^(m-k-1)/(m-k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A277458_val(idx_t m, seq_t *f)
{
    if (m == (idx_t)1) return (seq_t)1;
    /* f[k] = m*(m-1)*...*(m-k+1) */
    f[0] = (seq_t)1;
    for (idx_t k = 1; k <= m; ++k)
        f[k] = f[k - 1] * (seq_t)(m - (k - 1));
    seq_t result = (seq_t)0;
    seq_t pw = (seq_t)1; /* m^(m-k-1) */
    for (idx_t k = m; k >= 1; --k)
    {
        seq_t term;
        if (k == m)
            term = f[m];
        else
        {
            term = (seq_t)k * f[k] * pw;
            pw *= (seq_t)m;
        }
        if (k & (idx_t)1) result += term;
        else result -= term;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t *A = mem_new(seq_t, (size_t)n + 2);
    seq_t *row = mem_new(seq_t, (size_t)n + 2);
    seq_t *f = mem_new(seq_t, (size_t)n + 2);

    for (idx_t m = 1; m <= n; ++m)
        A[m] = A277458_val(m, f);

    /* Pascal row n-1 */
    row[0] = (seq_t)1;
    for (idx_t r = 1; r + 1 <= n; ++r)
        for (idx_t j = r; j >= 1; --j)
            row[j] += row[j - 1];

    seq_t total = (seq_t)0;
    seq_t pw = (seq_t)1; /* n^(n-k) */
    for (idx_t k = n; k >= 1; --k)
    {
        total += pw * row[k - 1] * A[k];
        pw *= (seq_t)n;
    }

    mem_free(A);
    mem_free(row);
    mem_free(f);
    return total;
}
