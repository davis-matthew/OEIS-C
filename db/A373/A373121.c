/*
 * A373121  Expansion of B(x)^2, where B(x) is the g.f. of A230322.
 * https://oeis.org/A373121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] B(x)^2, B(x) = Prod_{k>=1} (1-x^(7k-3))(1-x^(7k-4))
 *            / ((1-x^(7k-2))(1-x^(7k-5))) (g.f. of A230322)
 * Range:     i8 n <= 42, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    idx_t len = n + 1;
    seq_t *c = mem_new(seq_t, (size_t)len);
    if (!c) return (seq_t)0;

    c[0] = 1;
    static const int es[7] = {0, -1, 1, 1, -1, 0, 0};

    for (idx_t m = 1; m <= n; ++m) {
        int e = es[(m - 1) % 7];
        if (e == 0) continue;
        if (e > 0) {
            for (idx_t i = n; i >= m; --i)
                c[i] -= c[i - m];
        } else {
            for (idx_t i = m; i <= n; ++i)
                c[i] += c[i - m];
        }
    }

    seq_t res = 0;
    for (idx_t i = 0; i <= n; ++i)
        res += c[i] * c[n - i];

    mem_free(c);
    return res;
}
