/*
 * A324300  G.f.: Sum_{n>=0} x^n * (x^(n+1) + i)^n / (1 + i*x^(n+1))^(n+1).
 * https://oeis.org/A324300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is computed by enumerating the divisors d of n+1. For each
 *            divisor d, let m = d-1 and k = (n+1)/d - 1. If m+k is odd the
 *            contribution is 0. Otherwise we sum over a = 0..min(m,k): sign =
 *            (-1)^{ (k-a)+(m+k-2a)/2 } term = C(m,a) * C(m + k - a, k - a)
 *            The inner sum yields b(m,k); a(n) is the sum...
 * Range:     i8 n <= 19, i16 n <= 58, i32 n <= 193, i64 n <= 723,
 *            i128 n <= 2701, big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    idx_t N = n + 1;
    /* Collect divisors of N.  N ≤ 1e4+1, number of divisors < 256. */
    idx_t divs[256];
    size_t div_cnt = 0;
    for (idx_t i = 1; i <= N / i; ++i) {
        if (N % i == 0) {
            divs[div_cnt++] = i;
            if (i != N / i)
                divs[div_cnt++] = N / i;
        }
    }

    seq_t total = 0;

    for (size_t di = 0; di < div_cnt; ++di) {
        idx_t d = divs[di];
        idx_t m = d - 1;             /* m = d-1 */
        idx_t k = N / d - 1;         /* k = (n+1)/d - 1 */

        /* Only even m+k give a real contribution */
        if ((m + k) & 1)
            continue;

        idx_t min_a = (m < k) ? m : k;
        seq_t inner = 0;

        for (idx_t a = 0; a <= min_a; ++a) {
            seq_t c1 = binom(m, a);
            seq_t c2 = binom(m + k - a, k - a);
            seq_t term = seq_mul(c1, c2);

            idx_t exp = (k - a) + (m + k - 2 * a) / 2;
            if (exp & 1)
                inner -= term;
            else
                inner += term;
        }
        total += inner;
    }

    return total;
}
