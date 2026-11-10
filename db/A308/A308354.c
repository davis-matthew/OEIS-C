/*
 * A308354  Number of (2k+1)-ary quasitrivial semigroups that have two neutral
 *          elements on an n-element set.
 * https://oeis.org/A308354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2)*A292932(n-2) for n >= 2, a(1) = 0;
 *            A292932(0)=1, A292932(m) = m*A292932(m-1) + 2*Sum_{k=0}^{m-2}
 *            binomial(m,k)*A292932(k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 410 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t A292932_a(idx_t n)
{
    seq_t *c = mem_new(seq_t, (size_t)n + 1);
    seq_t res;
    c[0] = 1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t C = 1;
        for (idx_t k = 0; k + 1 < m; ++k) {
            sum += C * c[k];
            C = C * ((seq_t)(m - k)) / ((seq_t)(k + 1));
        }
        c[m] = ((seq_t)m) * c[m - 1] + ((seq_t)2) * sum;
    }
    res = c[n];
    mem_free(c);
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    seq_t b = (seq_t)n * ((seq_t)n - 1) / 2;
    return b * A292932_a(n - 2);
}
