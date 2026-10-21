/*
 * A244558  Numbers obtained by concatenating the squares of the digits of
 *          Fibonacci(n).
 * https://oeis.org/A244558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A048385(A000045(n))
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 30, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   fib, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the concatenation of the squares of the decimal
   digits of the n‑th Fibonacci number. */

#include <stddef.h>
#include "../utils/fib.h"
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    /* Get the n-th Fibonacci number.  The harness supplies fib. */
    seq_t f = fib(n);

    /* Buffer large enough for Fibonacci(10000) (~2090 decimal digits). */
    uint8_t digits[4000];
    size_t len;

    /* Extract the decimal digits of f into digits[] (least significant first). */
    if (f == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        get_digits(f, digits, &len);
    }

    /* Concatenate the squares of the digits. */
    seq_t result = 0;
    for (size_t i = len; i > 0; --i) {
        uint8_t d  = digits[i - 1];
        seq_t sq   = (seq_t)d * (seq_t)d;          /* 0 … 81 */
        seq_t shift = (sq < 10) ? (seq_t)10 : (seq_t)100;
        result = result * shift + sq;
    }

    return result;
}
