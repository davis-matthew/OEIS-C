/*
 * A357096  Least number whose set of decimal digits coincides with the set of
 *          decimal digits of prime(n).
 * https://oeis.org/A357096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the least positive integer whose set of decimal digits
 *            equals the set of decimal digits of prime(n)
 * Range:     u8 n <= 54, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   get_prime, min_from_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/min_from_mask.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is 1‑based per OEIS definition */
    uint32_t p = get_prime(n);      /* prime(n) as uint32_t */
    unsigned int mask = 0;
    uint32_t tmp = p;
    while (tmp > 0) {
        mask |= 1U << (tmp % 10);
        tmp /= 10;
    }
    return min_from_mask(mask);
}
