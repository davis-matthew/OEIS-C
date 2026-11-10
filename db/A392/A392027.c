/*
 * A392027  Partial sums of A255933.
 * https://oeis.org/A392027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} A255933(i); A255933(1)=1, A255933(k) =
 *            largest m with (m!-1) dividing Sum_{i<k} A255933(i) (computed by
 *            direct search over m)
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9953 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A392027_cum = NULL;
static size_t A392027_cap = 0;
static idx_t A392027_len = 0;

static void A392027_ensure(idx_t n)
{
    if ((size_t)n + 1 <= A392027_cap)
        return;
    size_t newcap = (A392027_cap == 0) ? 16 : A392027_cap;
    while (newcap < (size_t)n + 1)
        newcap <<= 1;
    seq_t *c = mem_resize(seq_t, A392027_cum, newcap);
    if (!c)
        abort();
    A392027_cum = c;
    A392027_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (A392027_len == 0) {
        A392027_ensure(1);
        A392027_cum[0] = (seq_t)0;
        A392027_cum[1] = (seq_t)1;
        A392027_len = 1;
    }
    if (n > A392027_len) {
        A392027_ensure(n);
        for (idx_t i = A392027_len + 1; i <= n; ++i) {
            seq_t S = A392027_cum[i - 1];
            seq_t best = (seq_t)1;
            seq_t fac = (seq_t)1;
            for (idx_t m = 2;; ++m) {
                /* stop if fac*m - 1 > S, without overflowing */
                if (fac > (S + 1) / (seq_t)m)
                    break;
                fac *= (seq_t)m;
                seq_t d = fac - 1;
                if (S % d == 0)
                    best = (seq_t)m;
            }
            A392027_cum[i] = A392027_cum[i - 1] + best;
        }
        A392027_len = n;
    }
    return A392027_cum[n];
}
