/*
 * A369265  Expansion of (1/x) * Series_Reversion( x * (1-x)^2 / (1+x^3) ).
 * https://oeis.org/A369265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * Σ_{k=0..⌊n/3⌋} C(n+1,k) * C(3n-3k+1, n-3k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n ≥ 0 and has no small‐n offset. */
#include <stdbool.h>
#include "../utils/seq_gcd.h"

/* Compute binomial coefficient C(a,b) using only seq_t arithmetic.
   The routine cancels intermediate factors with gcds to keep values
   small.  It works for any seq_t that supports +, -, *, /, and
   comparison operators. */
static seq_t
binom_multiplicative(idx_t a, idx_t b)
{
    if (b > a)
        return 0;
    if (b > a - b)
        b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        /* Multiply by (a - b + i) and divide by i, cancelling common factors. */
        seq_t num = (seq_t)(a - b + i);
        seq_t den = (seq_t)i;
        seq_t g = seq_gcd(num, den);
        num /= g;
        den /= g;

        g = seq_gcd(res, den);
        res /= g;
        den /= g;               /* den should now be 1 */

        res *= num;
    }
    return res;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 1;               /* Σ_{k=0..0} C(1,k)C(1,0)=1; divide by 1 → 1 */

    const idx_t maxk = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t c1 = binom_multiplicative(n + 1, k);
        seq_t c2 = binom_multiplicative(3 * n - 3 * k + 1, n - 3 * k);
        sum += c1 * c2;
    }
    return sum / (seq_t)(n + 1);
}
