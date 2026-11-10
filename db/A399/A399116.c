/*
 * A399116  Expansion of e.g.f. 1/(cosh(x) - x*cosh(x)^3).
 * https://oeis.org/A399116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(n,k) * a(k) * b(n-k),
 *            where b(n) = n*A054879((n-1)/2) = n*(9^((n-1)/2)+3)/4 for odd n
 *            and b(n) = -1 for even n >= 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    seq_t *av = mem_new(seq_t, N + 1);
    seq_t *bv = mem_new(seq_t, N + 2);
    seq_t *row = mem_new(seq_t, N + 2);
    size_t m, k, j;

    /* b for odd indices */
    seq_t pow9 = (seq_t)1;
    for (m = 1; m <= N; m += 2) {
        bv[m] = (seq_t)m * ((pow9 + (seq_t)3) / (seq_t)4);
        pow9 = pow9 * (seq_t)9;
    }

    av[0] = (seq_t)1;
    row[0] = (seq_t)1;
    for (m = 1; m <= N; m++) {
        for (j = m; j >= 1; j--)
            row[j] = row[j] + row[j - 1];
        seq_t pos = (seq_t)0, neg = (seq_t)0;
        for (k = 0; k < m; k++) {
            seq_t t = row[k] * av[k];
            if ((m - k) & 1)
                pos = pos + t * bv[m - k];
            else
                neg = neg + t;
        }
        av[m] = pos - neg;
    }

    seq_t res = av[N];
    mem_free(av);
    mem_free(bv);
    mem_free(row);
    return res;
}
