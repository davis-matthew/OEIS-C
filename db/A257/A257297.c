/*
 * A257297  a(n) = (initial digit of n) * (n with initial digit removed).
 * https://oeis.org/A257297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (initial digit of n) * (n with initial digit removed)
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 10, a(n) = 0 (since removing the only digit yields 0). */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    /* For all single‑digit numbers (including 0) the result is 0. */
    if (n < 10)
        return 0;

    /* Number of decimal digits of n. */
    int d = digits(n);

    /* 10^(d-1) fits in 64 bits for all n that fit in uint64_t. */
    uint64_t pow10d1 = (uint64_t)pow10(d - 1);

    /* Most significant (initial) digit. */
    uint64_t msd = n / pow10d1;

    /* Remaining part of n after removing the initial digit. */
    uint64_t rem = n % pow10d1;

    /* Multiply in the general output type seq_t. */
    return (seq_t)msd * (seq_t)rem;
}
