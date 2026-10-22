/*
 * A375214  Odd numbers with equal numbers of even and odd digits.
 * https://oeis.org/A375214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375214: a(n) is the nth odd integer with an even number of decimal digits,
   having equal numbers of even and odd digits. */

#include <stdbool.h>

static bool equal_even_odd_digits(seq_t x)
{
    idx_t odd = 0, even = 0, digits = 0;
    seq_t y = x;
    while (y > 0) {
        seq_t d = y % 10;
        if (d % 2 == 0)
            even++;
        else
            odd++;
        y /= 10;
        digits++;
    }
    return (digits & 1ULL) == 0 && even == odd;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t x = 21;                    /* first candidate (21) */
    for (;;) {
        if (equal_even_odd_digits(x)) {
            ++found;
            if (found == n)
                return x;
        }
        x += 2;                      /* next odd number */
    }
}
