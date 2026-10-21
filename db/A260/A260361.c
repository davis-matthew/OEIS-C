/*
 * A260361  G.f.: Sum_{n=-oo..+oo} x^n * (1 + x^n)^n, an even function.
 * https://oeis.org/A260361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^{2n} in S(x) = Σ_{m∈ℤ} x^m (1+x^m)^m
 *            which can be computed by a(n) = Σ_{m|2n, m>0} [ I[2n/m ≤ m+1] *
 *            C(m, 2n/m-1) + I[2n/m ≥ m-1] * (-1)^{2n/m-(m-1)} *
 *            C(m+2n/m-(m-1)-1, 2n/m-(m-1)) ] + (n==0 ? 2 : 0) Here I[cond] is
 *            1 if cond holds, else 0
 * Range:     u8 n <= 17, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <assert.h>

/* Small helper: binomial coefficient C(n,k) for unsigned integers.
   Assumes the result fits in uint64_t. */
static uint64_t binom_uint64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 2;          /* a(0) = 2 */

    idx_t two_n = 2*n;              /* 2n is the exponent we need */
    __int128 sum = 0;               /* use signed 128‑bit to allow subtractions */

    for (idx_t d = 1; d * d <= two_n; ++d) {
        if (two_n % d != 0) continue;

        /* first divisor d */
        {
            idx_t m = d;
            idx_t k1 = two_n / m;   /* 2n / m */

            if (k1 <= m + 1) {
                uint64_t add = binom_uint64(m, k1 - 1);
                sum += add;
            }

            if (k1 >= m - 1) { /* ensures k2 >= 0 */
                idx_t k2 = k1 - (m - 1);
                uint64_t add = binom_uint64(m + k2 - 1, k2);
                if ((k2 & 1) == 0)
                    sum += add;
                else
                    sum -= add;
            }
        }

        /* second divisor m = two_n / d, skip if equal to d */
        if (d != two_n / d) {
            idx_t m = two_n / d;
            idx_t k1 = two_n / m;   /* will be d */

            if (k1 <= m + 1) {
                uint64_t add = binom_uint64(m, k1 - 1);
                sum += add;
            }

            if (k1 >= m - 1) {
                idx_t k2 = k1 - (m - 1);
                uint64_t add = binom_uint64(m + k2 - 1, k2);
                if ((k2 & 1) == 0)
                    sum += add;
                else
                    sum -= add;
            }
        }
    }

    /* sum must be non‑negative by definition */
    assert(sum >= 0);
    return (seq_t)sum;
}
