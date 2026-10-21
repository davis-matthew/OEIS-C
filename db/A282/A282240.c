/*
 * A282240  a(n) = Fibonacci(n) represented in bijective base-9 numeration.
 * https://oeis.org/A282240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A052382(A000045(n))
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 45, u64 n <= 89, u128 n <= 177,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th Fibonacci number written as a bijective
   base‑9 integer.  The function uses the supplied `fib` utility. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Retrieve the n‑th Fibonacci number (F(1)=1, F(2)=1). */
    seq_t fn = fib(n);

    /* Temporary buffer for the bijective base‑9 digits.  A 10‑thousand
       Fibonacci number has fewer than 4000 decimal digits, so 4096 is
       ample room even when seq_t is 128‑bit. */
    uint8_t digits[4096];
    size_t sz = 0;

    /* Convert fn to bijective base‑9, storing least‑significant digit
       first.  The bijective encoding of a positive integer x is:
       x' = x - 1; digit = x' mod 9 + 1; x' = x' / 9. */
    seq_t tmp = fn;
    while (tmp > 0) {
        tmp--;                                 /* shift for bijective digit */
        digits[sz++] = (uint8_t)(tmp % 9 + 1);  /* digit in 1..9 */
        tmp /= 9;
    }

    /* Build the decimal integer whose decimal digits are the base‑9
       digits just computed. */
    seq_t result = 0;
    for (size_t i = sz; i > 0; --i) {
        result = result * 10 + (seq_t)digits[i - 1];
    }

    return result;
}
