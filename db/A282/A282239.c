/*
 * A282239  a(n) = Fibonacci(n) represented in bijective base-8 numeration.
 * https://oeis.org/A282239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A214678(A000045(n)) – Fibonacci(n) in bijective base‑8
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 85, u128 n <= 168,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* size_t */
#include <stdint.h>    /* uint8_t */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Fibonacci(n) */
    seq_t m = fib(n);

    /* Buffer for bijective base‑8 digits. 5000 is ample for n ≤ 10000. */
    uint8_t digits[5000];
    size_t cnt = 0;

    /* Convert m to bijective base‑8.  The loop follows the standard
       bijective‑base algorithm: subtract 1, take modulo 8, add 1. */
    while (m > 0)
    {
        m = m - 1;
        digits[cnt++] = (uint8_t)((m % 8) + 1);
        m = m / 8;
    }

    /* Compose the decimal number from the collected digits (most significant
       digit first). */
    seq_t res = 0;
    for (size_t i = cnt; i-- > 0;)
    {
        res = res * 10 + digits[i];
    }

    return res;
}
