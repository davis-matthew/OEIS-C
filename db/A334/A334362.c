/*
 * A334362  Number of r X s rectangles with integer sides such that r <= s, r
 *          + s = 2n and s/r is squarefree.
 * https://oeis.org/A334362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ i | 1 ≤ i ≤ n, 2n divisible by i, and μ(2n/i – 1) ≠ 0
 *            }
 * Range:     u8 n <= 90, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9674 (2026-11-01)
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
    seq_t total = 0;
    seq_t two_n = ((seq_t)n) * 2;          /* cast to seq_t to avoid overflow */
    for (idx_t i = 1; i <= n; ++i)
    {
        if ((two_n % (seq_t)i) == 0)       /* only consider divisors of 2n */
        {
            idx_t t = (idx_t)((two_n / (seq_t)i) - 1);
            if (mu_of(t) != 0)             /* μ(t)≠0 ⇔ t is squarefree */
                ++total;
        }
    }
    return total;
}
