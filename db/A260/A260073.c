/*
 * A260073  Number of triples {x, y, x mod y} of three distinct divisors of n.
 * https://oeis.org/A260073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples {x, y, x mod y} of three distinct
 *            divisors of n
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Collect all divisors of n (at most 256 for n up to 10⁴, enough for the intended limits). */
    idx_t divs[256];
    size_t dcnt = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            if (i != n / i)
                divs[dcnt++] = n / i;
        }
    }

    seq_t count = 0;

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t x = divs[i];
        for (size_t j = 0; j < dcnt; ++j) {
            if (i == j)
                continue;               /* x and y must be distinct */
            idx_t y = divs[j];
            idx_t r = x % y;
            if (r == 0 || r == x || r == y)
                continue;               /* remainder must be a distinct divisor */
            if (n % r == 0)            /* r divides n */
                ++count;
        }
    }

    return count;
}
