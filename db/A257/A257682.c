/*
 * A257682  Partial sums of A257680: a(0) = 0; for n >= 1, a(n) = A257680(n) +
 *          a(n-1).
 * https://oeis.org/A257682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A257680(k), a(0) = 0; A257680(k) = 1 if the
 *            factorial base representation of k contains a digit 1, else 0
 * Range:     u8 n <= 284, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 90 terms through n = 9542 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int A257680_a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t i = 1;
    while (n > 0)
    {
        idx_t d = n % (i + 1);
        if (d == 1)
            return 1;
        n = n / (i + 1);
        ++i;
    }
    return 0;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        if (A257680_a(k))
            ++s;
    }
    return s;
}
