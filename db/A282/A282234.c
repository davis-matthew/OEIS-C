/*
 * A282234  a(n) = Fibonacci(n) represented in bijective base-3 numeration.
 * https://oeis.org/A282234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Fibonacci(n) represented in bijective base-3 numeration
 *            (digits 1–3, interpreted as decimal digits)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* get the Fibonacci number */
    seq_t F = fib(n);

    /* array to store digits (least significant first) */
    size_t cap = 8192;          /* enough for n up to ~10 000 */
    uint8_t *dig = (uint8_t *)malloc(cap);
    if (!dig) return (seq_t)0;   /* fallback on allocation failure */

    size_t len = 0;
    seq_t X = F;

    while (X != 0) {
        /* digit in bijective base-3:
           r = X % 3; if r==0 -> r=3; X = (X - r) / 3; */
        seq_t r = X % (seq_t)3;
        if (r == 0) r = 3;
        if (len >= cap) {       /* grow if needed */
            cap *= 2;
            uint8_t *tmp = (uint8_t *)realloc(dig, cap);
            if (!tmp) { free(dig); return (seq_t)0; }
            dig = tmp;
        }
        dig[len++] = (uint8_t)r;
        X = (X - r) / (seq_t)3;
    }
    if (len == 0) {             /* handle F == 0 (not possible for n>=1) */
        free(dig);
        return 0;
    }

    /* convert digits to decimal number: digits[msb..lsb] */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        /* multiply by 10 then add next most significant digit */
        result = result * (seq_t)10 + (seq_t)dig[len - 1 - i];
    }

    free(dig);
    return result;
}
