/*
 * A326605  G.f.: Sum_{n>=0} (2*n + 1) * x^n * (9 - x^n)^n.
 * https://oeis.org/A326605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, 0 ≤ n/d−1 ≤ d} (2d+1) ⋅ C(d, n/d−1) ⋅
 *            9^{\,d−(n/d−1)} ⋅ (−1)^{\,n/d−1}, with a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

/* Return the unsigned value of the contribution of divisor d to a(n)
   and set *oddp to 1 if (n/d−1) is odd (so the term is negative), 0 otherwise.
   If the divisor d does not satisfy the condition k ≤ d it returns 0. */
static inline seq_t divisor_term(idx_t d, idx_t n, int *oddp) {
    idx_t k = n / d - 1U;          /*  k = n/d − 1  */
    if (k > d) {                   /* condition not met */
        *oddp = 0;
        return (seq_t)0;
    }
    *oddp = (k & 1U) ? 1 : 0;      /* sign */
    seq_t val = (seq_t)(2U * d + 1U);     /* first factor 2d+1 */
    val *= binomial(d, k);               /* binomial coefficient C(d,k) */
    val *= seq_pow((seq_t)9, d - k);     /* 9^{d−k} */
    return val;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t pos = 0;   /* sum of positive terms   */
    seq_t neg = 0;   /* sum of absolute values of negative terms */

    idx_t i;
    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            int odd;
            seq_t val = divisor_term(i, n, &odd);
            if (odd) neg += val;
            else    pos += val;

            idx_t j = n / i;
            if (j != i) {
                val = divisor_term(j, n, &odd);
                if (odd) neg += val;
                else    pos += val;
            }
        }
    }

    return pos - neg;   /* final result (always non‑negative) */
}
