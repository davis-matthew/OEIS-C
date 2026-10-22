/*
 * A347211  Square array T(n, k) read by ascending antidiagonals, T(n, k) =
 *          Sum_{j=0..n} (-1)^(n + j)*(6 - n + j)^k * binomial(12, n - j) if k
 *          > 0 and (-1)^n otherwise. T(n, k) for 0 <= n, k <= 11.
 * https://oeis.org/A347211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign, tabf, fini
 * Formula:   a(n) = T(n,k) read by ascending antidiagonals, where T(n,k) =
 *            Sum_{j=0..n} (-1)^(n+j)*(6 - n + j)^k * binomial(12, n - j) if k
 *            > 0, and T(n,k) = (-1)^n if k = 0, for 0 <= n,k <= 11
 * Range:     i8 n <= 6, i16 n <= 26, i32 n <= 143, i64 n <= 143,
 *            i128 n <= 143, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

static seq_t pow_seq_int(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t e = 0; e < exp; ++e)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    /* The square array has 12×12 = 144 entries.  For n outside this
       range the sequence is undefined; returning 0 is safe. */
    if (n >= 144)
        return 0;

    idx_t rem = n;
    idx_t d   = 0;
    while (true) {
        idx_t len = (d <= 11) ? (d + 1) : (23 - d);
        if (rem < len) {          /* found the diagonal d that contains n */
            idx_t m = rem;        /* position within diagonal d   */
            idx_t i, k;
            if (d <= 11) {
                i = d - m;
                k = m;
            } else {
                i = 11 - m;
                k = d - 11 + m;
            }

            /* Non‑zero only for 0 <= i,k <= 11 */
            if (k == 0)
                return (seq_t)((i & 1) ? -1 : 1);

            seq_t sum = 0;
            for (idx_t j = 0; j <= i; ++j) {
                int parity = ((i + j) & 1) ? -1 : 1;
                int64_t base_int = (int64_t)6 - (int64_t)i + (int64_t)j;
                seq_t base   = (seq_t)base_int;
                seq_t powv   = pow_seq_int(base, k);
                seq_t coeff  = binom(12, i - j);
                seq_t term   = powv * coeff;
                if (parity < 0)
                    term = -term;
                sum += term;
            }
            return sum;
        }
        rem -= len;
        ++d;
    }
}
