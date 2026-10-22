/*
 * A372459  Coefficient of x^n in the expansion of 1 / ( (1-x) * (1-x-x^3)^2
 *          )^n.
 * https://oeis.org/A372459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n+k-1, k) *
 *            binomial(4*n-2*k-1, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

/* multiplicative binomial that works for both 64‑bit and arbitrary precision seq_t */
static seq_t binomial_safe(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;          /* numerator term */
        seq_t den = i;                  /* denominator term */

        /* cancel common factors with current result */
        seq_t g = gcd(result, den);
        result /= g;
        den /= g;

        /* cancel common factors between remaining numerator and denominator */
        g = gcd(num, den);
        num /= g;
        den /= g;

        /* multiply and divide */
        result = result * num;
        /* den should now be 1, but divide to be safe */
        result /= den;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }

    seq_t sum = 0;
    idx_t max_k = n / 3u;
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t bin1 = binomial_safe(2 * n + k - 1, k);
        seq_t bin2 = binomial_safe(4 * n - 2 * k - 1, n - 3 * k);
        sum += bin1 * bin2;
    }
    return sum;
}
