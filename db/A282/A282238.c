/*
 * A282238  a(n) = Fibonacci(n) represented in bijective base-7 numeration.
 * https://oeis.org/A282238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A214677(A000045(n)) (bijective base-7 representation of
 *            Fibonacci(n))
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 80, u128 n <= 157,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include <stdint.h>

/* Return the Fibonacci number F(n) expressed in bijective base‑7 and then
   interpreted as a decimal integer.  For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* fibonacci number */
    seq_t f = fib(n);

    /* array to hold digits (each digit is 1..7).  5000 is ample for n up to ~10^4. */
    uint8_t digits[5000];
    int cnt = 0;

    /* convert to bijective base‑7: repeatedly decrement, mod 7, then divide by 7. */
    while (f > 0) {
        f = f - 1;                        /* make remainder range 0..6 */
        int digit = (int)(f % 7) + 1;      /* digit in 1..7 */
        digits[cnt++] = (uint8_t)digit;
        f = f / 7;
    }

    /* build the decimal integer from most significant digit to least. */
    seq_t result = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }

    return result;
}
