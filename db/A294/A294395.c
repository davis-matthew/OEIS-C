/*
 * A294395  E.g.f.: exp(Sum_{n>=1} A050999(n) * x^n).
 * https://oeis.org/A294395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} (n-1)!/(n-k)! *
 *            k*A050999(k)*a(n-k) for n > 0, where A050999(k) = sum of squares
 *            of odd divisors of k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 368 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *e294395_tab = 0;
static seq_t *e294395_c = 0;
static idx_t e294395_cnt = 0;

static seq_t e294395_odd_sq(idx_t k)
{
    seq_t s = 0;
    for (idx_t d = 1; d <= k; d += 2) {
        if (k % d == 0) s += (seq_t)d * (seq_t)d;
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n >= e294395_cnt) {
        idx_t newcnt = n + 1;
        if (e294395_cnt == 0) {
            e294395_tab = mem_new(seq_t, newcnt);
            e294395_c = mem_new(seq_t, newcnt);
        } else {
            e294395_tab = mem_resize(seq_t, e294395_tab, newcnt);
            e294395_c = mem_resize(seq_t, e294395_c, newcnt);
        }
        for (idx_t m = e294395_cnt; m < newcnt; ++m) {
            if (m == 0) {
                e294395_tab[0] = 1;
                e294395_c[0] = 0;
                continue;
            }
            e294395_c[m] = e294395_odd_sq(m);
            seq_t total = 0;
            seq_t f = 1;
            for (idx_t k = 1; k <= m; ++k) {
                if (k >= 2) f *= (seq_t)(m - k + 1);
                total += f * (seq_t)k * e294395_c[k] * e294395_tab[m - k];
            }
            e294395_tab[m] = total;
        }
        e294395_cnt = newcnt;
    }
    return e294395_tab[n];
}
