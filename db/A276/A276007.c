/*
 * A276007  a(n) = number of nonzero digits in factorial base representation
 *          of n that hit less significant nonzero digits to the right. See
 *          comments for exact definition.
 * https://oeis.org/A276007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8072, u16 n <= 8072, u32 n <= 8072, u64 n <= 8072,
 *            u128 n <= 8072, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A276007: for a number n,
   it converts n to its factorial-base representation
   and counts the non‑zero digits that "hit" another
   non‑zero digit to their right (i.e. the digit at
   position i has value d_i > 0 and the digit at
   position i - d_i is also non‑zero). */

#include <stddef.h>

seq_t a(idx_t n_input)
{
    if (n_input == 0)
        return 0;

    /* n will be divided by successive bases.  The
       number of factorial-base digits needed for a
       64‑bit integer is at most 21 (since 21! > 2^64).*/
    idx_t n = n_input;
    idx_t digits[25];   /* fixed upper bound (<=21) */
    idx_t len = 0;

    for (idx_t base = 2; ; ++base) {
        digits[len++] = n % base;
        n /= base;
        if (n == 0)
            break;
    }

    seq_t count = 0;

    /* positions are 1‑based: digit at index i‑1
       corresponds to position i. */
    for (idx_t pos = 1; pos <= len; ++pos) {
        idx_t d = digits[pos - 1];
        if (d == 0)
            continue;

        idx_t target = pos - d;
        if (target >= 1 && digits[target - 1] != 0)
            ++count;
    }

    return count;
}
