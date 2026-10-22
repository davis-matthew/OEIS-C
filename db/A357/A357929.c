/*
 * A357929  Numbers that share a (decimal) digit with at least 1 of their
 *          proper divisors.
 * https://oeis.org/A357929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A357929: numbers that share a decimal digit with at least one
   proper divisor.  The function performs a straightforward incremental
   search; no closed formula is known. */

#include <stdbool.h>

/* Return a 10‑bit mask of the decimal digits present in x (0‑9). */
static unsigned int digit_mask(seq_t x)
{
    unsigned int mask = 0;
    if (x == 0) {
        mask = 1u << 0;          /* digit 0 */
    }
    while (x != 0) {
        unsigned int d = (unsigned int)(x % 10);
        mask |= 1u << d;
        x /= 10;
    }
    return mask;
}

/* Test whether x has a common decimal digit with at least one proper
   divisor of x. */
static bool shares_digit_with_divisor(seq_t x)
{
    unsigned int mask_x = digit_mask(x);
    for (seq_t d = 1; d * d <= x; ++d) {
        if (x % d == 0) {
            seq_t d1 = d;
            if ((mask_x & digit_mask(d1)) != 0) {
                return true;
            }
            seq_t d2 = x / d;
            /* d2 == x only when d == 1; skip x itself. */
            if (d2 < x && (mask_x & digit_mask(d2)) != 0) {
                return true;
            }
        }
    }
    return false;
}

/* Public sequence function.  a(1) is the first value 10. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for index 0; return 0 as a sentinel. */
        return 0;
    }

    seq_t candidate = 10;   /* smallest number with a proper divisor (>1) */
    idx_t count = 0;        /* how many valid numbers found so far */

    while (count < n) {
        if (shares_digit_with_divisor(candidate)) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
        ++candidate;
    }

    /* Should never reach here. */
    return 0;
}
