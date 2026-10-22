/*
 * A345036  a(n) = Sum_{k=1..n} (-n)^(floor(n/k) - 1).
 * https://oeis.org/A345036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-n)^(floor(n/k)-1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1; returning 0 for n==0 is harmless. */
    if (n == 0) return 0;

    seq_t sum = 0;
    seq_t base = (seq_t)(-1) * (seq_t)n;          /* base = -n */

    for (idx_t k = 1; k <= n; ) {
        idx_t m = n / k;                          /* current value of floor(n/k)   */
        idx_t nextk = n / m + 1;                  /* first k where floor(n/k) drops */
        idx_t count = nextk - k;                  /* number of k's with this m     */

        seq_t pow_val = seq_pow(base, m - 1);     /* (-n)^(m-1) */
        seq_t term = pow_val * (seq_t)count;      /* contribution of this block   */

        sum += term;
        k = nextk;
    }

    return sum;
}
