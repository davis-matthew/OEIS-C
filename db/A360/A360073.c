/*
 * A360073  a(n) is the greatest divisor of n divisible by the product of its
 *          own digits.
 * https://oeis.org/A360073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the greatest divisor of n that is divisible by the
 *            product of its own decimal digits
 * Range:     u8 n <= 311, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset safety – not required for tests */
        return 0;

    seq_t best = 1;      /* 1 always satisfies the test */

    /* Enumerate divisors of n.  Use i up to sqrt(n).  */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0)
            continue;

        /* first divisor i */
        seq_t d = (seq_t)i;
        seq_t prod = digit_product(d);
        if (prod != 0 && d % prod == 0 && d > best)
            best = d;

        /* paired divisor n / i */
        idx_t j = n / i;
        if (j == i)
            continue;

        seq_t d2 = (seq_t)j;
        seq_t prod2 = digit_product(d2);
        if (prod2 != 0 && d2 % prod2 == 0 && d2 > best)
            best = d2;
    }

    return best;
}
