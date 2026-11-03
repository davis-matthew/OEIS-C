/*
 * A346761  a(n) = Sum_{d|n} mu(n/d) * binomial(d,4).
 * https://oeis.org/A346761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(n/d) * binomial(d,4)
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset starts at 1 */

    long long total = 0;                /* signed accumulator */

    for (idx_t i = 1; i <= n / i; ++i)  /* iterate over divisor pairs */
    {
        if (n % i != 0) continue;

        idx_t d1 = i;             /* first divisor */
        idx_t d2 = n / i;         /* complementary divisor */

        /* process d1 */
        if (d1 >= 4) {
            int mu = mu_of(n / d1);
            if (mu != 0) {
                unsigned long long bin_ll = (unsigned long long)d1 *
                                            (d1 - 1) *
                                            (d1 - 2) *
                                            (d1 - 3) / 24ULL;
                seq_t bin = (seq_t)bin_ll;
                total += mu * (long long)bin;
            }
        }

        /* process d2, if distinct */
        if (d2 != d1 && d2 >= 4) {
            int mu = mu_of(n / d2);
            if (mu != 0) {
                unsigned long long bin_ll = (unsigned long long)d2 *
                                            (d2 - 1) *
                                            (d2 - 2) *
                                            (d2 - 3) / 24ULL;
                seq_t bin = (seq_t)bin_ll;
                total += mu * (long long)bin;
            }
        }
    }

    return (seq_t)total;   /* result is always non‑negative for n ≥ 1 */
}
