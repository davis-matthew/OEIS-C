/*
 * A371351  Number of achiral polyominoes composed of n tetrahedral cells of
 *          the hyperbolic regular tiling with Schläfli symbol {3,3,oo}.
 * https://oeis.org/A371351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ([0==n mod 2]*2*C(3n/2,n) + [1==n mod 2]*3*C((3n-1)/2,n)
 *            * + [1==n mod4]*3*C((3n-3)/4,(n-1)/2) * + [2==n
 *            mod6]*3*C(n/2-1,(n-2)/3)) / (3n+3)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiplicative binomial coefficient: C(n,k) for any seq_t type. */
static seq_t binomial_calc(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide – the division is exact */
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 – return 0 */
    seq_t sum = 0;

    if ((n & 1) == 0) {
        /* n even: 2*C(3n/2, n) */
        sum += (seq_t)2 * binomial_calc((idx_t)(3 * n / 2), n);
    } else {
        /* n odd: 3*C((3n-1)/2, n) */
        sum += (seq_t)3 * binomial_calc((idx_t)((3 * n - 1) / 2), n);
    }

    if (n % 4 == 1) {
        /* extra term when n ≡ 1 (mod 4) */
        sum += (seq_t)3 * binomial_calc((idx_t)((3 * n - 3) / 4),
                                         (idx_t)((n - 1) / 2));
    }

    if (n % 6 == 2) {
        /* extra term when n ≡ 2 (mod 6) */
        sum += (seq_t)3 * binomial_calc((idx_t)(n / 2 - 1),
                                         (idx_t)((n - 2) / 3));
    }

    seq_t denom = (seq_t)(3 * n + 3);
    return sum / denom;
}
