/*
 * A351781  a(n) = (n-1)^n*(n-1)!^n.
 * https://oeis.org/A351781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)^n * (n-1)!^n
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle the case before the defined offset */
    if (n == 0) return 0;

    /* Compute (n-1)! */
    idx_t m = n - 1;
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; ++i) {
        fact *= (seq_t)i;
    }

    /* Base = (n-1) * (n-1)! */
    seq_t base = fact * (seq_t)m;

    /* a(n) = base^n  (i.e. (n-1)^n * (n-1)!^n) */
    return pow_seq(base, n);
}
