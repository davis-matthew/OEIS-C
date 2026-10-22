/*
 * A346576  Let x run through the list of numbers with no zeros (A052382);
 *          replace each digit d of x by the digit (x mod d).
 * https://oeis.org/A346576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = concatenation of (x mod d) for each decimal digit d of
 *            the nth positive integer x whose decimal representation contains
 *            no zero digit (zerofree integers)
 * Range:     u8 n <= 312, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Test whether an integer has a zero digit.  */
static bool has_zero_digit(idx_t x)
{
    while (x != 0)
    {
        if ((x % 10) == 0)
            return true;
        x /= 10;
    }
    return false;
}

/* Return the n‑th term of sequence A346576.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n < 1, but safe fallback */

    idx_t count = 0;                    /* number of zerofree integers seen */
    idx_t x = 1;                        /* current integer to test */

    /* Find the n‑th zero‑free integer.  */
    for (;; ++x)
    {
        if (!has_zero_digit(x))
        {
            ++count;
            if (count == n)
                break;
        }
    }

    /* Extract the decimal digits of x in normal order.  */
    unsigned char digits[32];            /* enough for all tested n       */
    size_t len = 0;
    idx_t t = x;
    while (t != 0)
    {
        digits[len++] = (unsigned char)(t % 10);
        t /= 10;
    }
    /* Reverse to get most‑significant‑digit first.  */
    for (size_t i = 0; i < len / 2; ++i)
    {
        unsigned char tmp = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmp;
    }

    /* Concatenate the remainders x mod d for each digit d.  */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i)
    {
        idx_t d = digits[i];
        seq_t rem = (seq_t)(x % d);      /* d is 1‑9, so remainder fits in seq_t */
        result = result * (seq_t)10 + rem;
    }

    return result;
}
