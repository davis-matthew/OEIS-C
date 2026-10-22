/*
 * A363977  Expansion of Sum_{k>0} x^k / (1 - x^(4*k))^3.
 * https://oeis.org/A363977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n, d==1 mod 4} binomial(((d+3)/4)+1, 2)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sequence element a(n) for OEIS A363977 */
seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if ((d1 & 3ULL) == 1) {
                seq_t t = ((seq_t)d1 + 3ULL) / 4ULL + 1;
                result += t * (t - 1ULL) / 2ULL;
            }
            idx_t d2 = n / i;
            if (d2 != d1 && (d2 & 3ULL) == 1) {
                seq_t t = ((seq_t)d2 + 3ULL) / 4ULL + 1;
                result += t * (t - 1ULL) / 2ULL;
            }
        }
    }
    return result;
}
