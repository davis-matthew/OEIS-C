/*
 * A400370  Deterministic version of the random Fibonacci sequence, with signs
 *          from A030190 and the seed as (1,0).
 * https://oeis.org/A400370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) + a(n-2) if A030190(n-2) = 1, else a(n-1) -
 *            a(n-2); a(0) = 1, a(1) = 0
 * Range:     i8 n <= 33, i16 n <= 51, i32 n <= 123, i64 n <= 301,
 *            i128 n <= 610, big any n
 * Validated: standard, pass, 69 terms through n = 9143 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* A030190: n-th bit of binary Champernowne word 0,1,2,3,... (computed in uint64_t) */
static unsigned A030190_bit(uint64_t n)
{
    if (n == 0)
        return 0;
    uint64_t rem = n - 1;
    unsigned len = 1;
    while (1) {
        uint64_t count = ((uint64_t)1) << (len - 1);
        uint64_t bits = count * len;
        if (rem < bits)
            break;
        rem -= bits;
        len++;
    }
    uint64_t index = rem / len;
    unsigned offset = (unsigned)(rem % len);
    uint64_t number = (((uint64_t)1) << (len - 1)) + index;
    unsigned shift = len - 1 - offset;
    return (unsigned)((number >> shift) & 1);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;
    seq_t p = 1, q = 0, r = 0; /* p = a(k-2), q = a(k-1) */
    for (idx_t k = 2; k <= n; k++) {
        if (A030190_bit(k - 2))
            r = q + p;
        else
            r = q - p;
        p = q;
        q = r;
    }
    return q;
}
