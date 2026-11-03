/*
 * A245760  Maximal multiplicative persistence of n in any base.
 * https://oeis.org/A245760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9728 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximal multiplicative persistence of n in any base 2 … n */
#include <stdbool.h>

/* Compute the product of all digits of x in the given base.  The
   loop iterates until x becomes 0, thus every digit—including the
   most‑significant one—is multiplied.  If a zero digit occurs the
   product becomes zero and the loop can terminate early. */
static seq_t product_digits(seq_t x, seq_t base)
{
    seq_t prod = 1;
    while (x > 0) {
        seq_t digit = x % base;
        if (digit == 0)
            return 0;           /* zero kills the product */
        prod *= digit;
        x /= base;
    }
    return prod;
}

/* Compute the multiplicative persistence of x in the supplied base.
   The function counts how many times we need to replace x by the
   product of its base‑digits until x becomes a single digit in that
   base. */
static seq_t persistence_in_base(seq_t x, seq_t base)
{
    idx_t steps = 0;
    while (x >= base) {
        x = product_digits(x, base);
        ++steps;
    }
    return (seq_t)steps;
}

/* Sequence function required by the build system. */
seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(1) = 0 by definition */
        return 0;

    seq_t max_persistence = 0;
    seq_t n_val = (seq_t)n;

    for (idx_t b = 2; b <= n; ++b) {
        seq_t p = persistence_in_base(n_val, (seq_t)b);
        if (p > max_persistence)
            max_persistence = p;
    }
    return max_persistence;
}
