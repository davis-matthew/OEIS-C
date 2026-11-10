/*
 * A400689  Row sums of the triangle A400688.
 * https://oeis.org/A400689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} T(n,k), where T(n,k) = [x^n] 1/(1 - 2*x -
 *            Sum_{h=1..k-1} A000108(h)*x^(h+1)), i.e. t(0)=1, t(m) = 2*t(m-1)
 *            + Sum_{h=1..min(k-1,m-1)} A000108(h)*t(m-h-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-10)
 * Helpers:   catalan, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t *c = mem_new(seq_t, n + 1);
    seq_t *t = mem_new(seq_t, n + 1);
    for (idx_t h = 1; h < n; ++h) c[h] = catalan(h);

    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        t[0] = 1;
        for (idx_t m = 1; m <= n; ++m) {
            seq_t s = 2 * t[m - 1];
            for (idx_t h = 1; h < k && h < m; ++h)
                s += c[h] * t[m - h - 1];
            t[m] = s;
        }
        total += t[n];
    }
    mem_free(c);
    mem_free(t);
    return total;
}
