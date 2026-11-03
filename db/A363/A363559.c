/*
 * A363559  Expansion of g.f. A(x) = Sum_{n=-oo..+oo} x^n * (3 + x^n)^(2*n).
 * https://oeis.org/A363559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n ≥ 1, a(n) = Σ_{d|n} [ (n/d ≤ 2d+1) * C(2d, n/d –
 *            1) * 3^{2d – (n/d – 1)} + (n/d ≥ 2d–1) * (–1)^{n/d – (2d–1)} *
 *            C(2d + (n/d – (2d–1)) – 1, n/d – (2d–1)) * 3^{n/d – (2d–1)} ]
 *            where C(a,b) is the binomial coefficient
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 290 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

/* Compute a(n) for A363559 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t pos = 0;   /* sum of positive‑contribution magnitudes   */
    seq_t neg = 0;   /* sum of negative‑contribution magnitudes  */

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* First divisor d = i */
            idx_t d = i;
            idx_t t = n / d;   /* n/d */

            /* Positive contribution */
            if (t <= 2 * d + 1) {
                idx_t k = t - 1;   /* j = n/d – 1 */
                seq_t bin = binomial((idx_t)(2 * d), k);
                seq_t pow3 = ipow((seq_t)3, (seq_t)(2 * d - k));
                pos += bin * pow3;
            }

            /* Negative contribution */
            if (t >= 2 * d - 1) {
                idx_t k = t - (2 * d - 1);   /* k = n/d – (2d–1) */
                seq_t binneg = binomial((idx_t)(2 * d + k - 1), k);
                seq_t pow3n = ipow((seq_t)3, (seq_t)k);
                seq_t term = binneg * pow3n;
                if ((k & 1) == 0) {
                    pos += term;        /* sign (+) */
                } else {
                    neg += term;        /* sign (–) */
                }
            }

            /* Partner divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d) {
                d = d2;
                t = n / d;   /* this equals i */

                /* Positive contribution for d2 */
                if (t <= 2 * d + 1) {
                    idx_t k = t - 1;
                    seq_t bin = binomial((idx_t)(2 * d), k);
                    seq_t pow3 = ipow((seq_t)3, (seq_t)(2 * d - k));
                    pos += bin * pow3;
                }

                /* Negative contribution for d2 */
                if (t >= 2 * d - 1) {
                    idx_t k = t - (2 * d - 1);
                    seq_t binneg = binomial((idx_t)(2 * d + k - 1), k);
                    seq_t pow3n = ipow((seq_t)3, (seq_t)k);
                    seq_t term = binneg * pow3n;
                    if ((k & 1) == 0) {
                        pos += term;
                    } else {
                        neg += term;
                    }
                }
            }
        }
    }

    return pos - neg;
}
