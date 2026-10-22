/*
 * A342933  a(n) = n! * Sum_{k=1..n} k^2/floor(n/k).
 * https://oeis.org/A342933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1..n} k^2/floor(n/k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute factorial of n in seq_t */
static seq_t factorial(idx_t n)
{
    seq_t f = 1;
    for (idx_t i = 2; i <= n; ++i)
        f *= i;
    return f;
}

/* Sequence function a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* under offset, not used */

    seq_t fact = factorial(n);
    seq_t sum  = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t d = n / k;                /* floor(n/k) */
        seq_t fact_div = fact / d;      /* n! / floor(n/k) */
        seq_t term = fact_div * (seq_t)k * (seq_t)k; /* *k^2 */
        sum += term;
    }

    return sum;
}
