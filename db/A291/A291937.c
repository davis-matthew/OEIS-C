/*
 * A291937  G.f.: Sum_{n=-oo..+oo} n * x^n * (1 - x^n)^n.
 * https://oeis.org/A291937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(0) = 1 For n > 0, let d run over all positive divisors of n.
 *            Let q = n / d. Positive‑side contribution (from m = d): if q <=
 *            d + 1, k = q - 1; term = d * binom(d, k) * (-1)^k (added to the
 *            sum). Negative‑side contribution (from t = d): if q >= d - 1,
 *            term = d * binom(q, d - 1) * (-1)^{d+1} (add...
 * Range:     i8 n <= 19, i16 n <= 74, i32 n <= 339, i64 n <= 1424,
 *            i128 n <= 5944, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;        /* a(0) = 1 */

    seq_t sum = 0;
    idx_t i;

    /* iterate over all divisor pairs (i, e) */
    for (i = 1; i * i <= n; ++i) {
        if (n % i != 0)
            continue;

        /* first divisor: d = i */
        idx_t d = i;
        idx_t q = n / d;          /* n/d */

        /* positive‑side contribution from d */
        if (q <= d + 1) {         /* k = q‑1 is between 0 and d */
            idx_t k = q - 1;
            seq_t term = binom(d, k);
            term *= (seq_t)d;
            if (k & 1)
                term = -term;
            sum += term;
        }

        /* negative‑side contribution from d */
        if (q >= d - 1) {         /* d‑1 <= q */
            seq_t term = binom(q, d - 1);
            term *= (seq_t)d;
            /* sign = (-1)^{d+1}  =>  negative when d is even */
            if ((d & 1) == 0)   /* d even */
                term = -term;
            sum += term;
        }

        /* second divisor: e = n / i , if distinct */
        idx_t e = n / i;
        if (e == d)
            continue;             /* already processed */

        d = e;
        q = n / d;                  /* which is i */

        /* positive‑side from e */
        if (q <= d + 1) {
            idx_t k = q - 1;
            seq_t term = binom(d, k);
            term *= (seq_t)d;
            if (k & 1)
                term = -term;
            sum += term;
        }

        /* negative‑side from e */
        if (q >= d - 1) {
            seq_t term = binom(q, d - 1);
            term *= (seq_t)d;
            if ((d & 1) == 0)
                term = -term;
            sum += term;
        }
    }

    return sum;
}
