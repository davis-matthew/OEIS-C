/*
 * A387463  Total number of 3's in the decimal digits of the divisors of n.
 * https://oeis.org/A387463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d | n} (# of decimal digit 3’s in d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses a simple divisor loop up to ⌊√n⌋; no external helpers required.  */

#include <stdint.h>
#include "../utils/count3.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for 0, but kept safe. */
    seq_t total = 0;

    /* Iterate over all divisors d with d ≤ n/d. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if ((n % d) == 0) {
            total += count3(d);          /* divisor d itself */

            idx_t d2 = n / d;            /* the paired divisor */
            if (d2 != d) {
                total += count3(d2);      /* add the distinct pair */
            }
        }
    }
    return total;
}
