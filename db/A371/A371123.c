/*
 * A371123  Numbers whose decimal representation contains the digital root of
 *          the product of their digits.
 * https://oeis.org/A371123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate over all non‑negative integers and return
   the n‑th number whose decimal representation contains the
   digital root of the product of its digits. */

#include "../utils/digit_product.h"

static bool contains_digit(seq_t x, unsigned d)
{
    if (x == 0) return d == 0;
    while (x > 0)
    {
        if ((x % 10) == d) return true;
        x /= 10;
    }
    return false;
}

static bool satisfies(seq_t x)
{
    seq_t prod = digit_product(x);
    unsigned dr;
    if (prod == 0)
        dr = 0;
    else
        dr = (unsigned)((prod - 1) % 9) + 1;  /* digital root in [1,9] */
    return contains_digit(x, dr);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for out‑of‑range calls */
    idx_t count = 0;
    seq_t x = 0;
    for (;; ++x)
    {
        if (satisfies(x))
        {
            ++count;
            if (count == n) return x;
        }
    }
}
