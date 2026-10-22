/*
 * A321521  a(n) = Sum_{d|n} (-1)^(n/d+1)*d!.
 * https://oeis.org/A321521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} (-1)^(n/d + 1) * d! (offset 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0) continue;           /* i not a divisor */
        idx_t d1 = i;
        idx_t d2 = n / i;                   /* complementary divisor */

        /* Contribution from d1: sign depends on parity of n/d1 = d2 */
        seq_t f1 = factorial(d1);
        if ((d2 & 1u) != 0) pos += f1; else neg += f1;

        if (d1 != d2) {
            /* Contribution from d2: sign depends on parity of n/d2 = d1 */
            seq_t f2 = factorial(d2);
            if ((d1 & 1u) != 0) pos += f2; else neg += f2;
        }
    }
    return pos - neg;   /* pos >= neg for all n ≥ 1, result is non‑negative */
}
