/*
 * A399952  Deterministic version of the random Fibonacci sequence, with signs
 *          from A030190 and the seed as (0,1).
 * https://oeis.org/A399952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) + a(n-2) if A030190(n-2) = 1, otherwise a(n-1) -
 *            a(n-2), for n >= 2; a(0) = 0, a(1) = 1
 * Range:     i8 n <= 31, i16 n <= 51, i32 n <= 122, i64 n <= 300,
 *            i128 n <= 604, big any n
 * Validated: standard, pass, 70 terms through n = 8591 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A030190_bit(uint64_t n)
{
    if (n == 0)
        return 0;
    uint64_t rem = n - 1;
    unsigned len = 1;
    for (;;) {
        uint64_t bits = (((uint64_t)1) << (len - 1)) * len;
        if (rem < bits)
            break;
        rem -= bits;
        len++;
    }
    uint64_t number = (((uint64_t)1) << (len - 1)) + rem / len;
    unsigned shift = len - 1 - (unsigned)(rem % len);
    return (unsigned)((number >> shift) & 1);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t x = 0, y = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t c;
        if (A030190_bit((uint64_t)(i - 2)))
            c = y + x;
        else
            c = y - x;
        x = y;
        y = c;
    }
    return y;
}
