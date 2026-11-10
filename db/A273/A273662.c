/*
 * A273662  Least monotonic left inverse for A256450: a(1) = 0; for n > 1,
 *          a(n) = A257680(n) + a(n-1).
 * https://oeis.org/A273662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(n) = A257680(n) + a(n-1) for n > 1, where A257680(k)
 *            = 1 if factorial base representation of k has a digit 1, else 0
 * Range:     u8 n <= 285, u16 n <= 10080, u32 n <= 10080, u64 n <= 10080,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 101 terms through n = 9093 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int A257680_has_one(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t i = 1;
    while (n > 0) {
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
    for (idx_t k = 2; k <= n; ++k)
        s += (seq_t)A257680_has_one(k);
    return s;
}
