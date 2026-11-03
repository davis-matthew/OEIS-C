/* fibonacci: the Fibonacci number F(n), F(0) = 0.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_FIBONACCI_H
#define OEISC_UTILS_FIBONACCI_H
#include <stdint.h>

/* The Fibonacci number F(n), F(0) = 0, by fast doubling:
   F(2k) = F(k) (2 F(k+1) - F(k)),  F(2k+1) = F(k)^2 + F(k+1)^2.
   Only +, - and * are used, so at a fixed width the result is F(n) modulo
   2^width, the same as summing F(i) = F(i-1) + F(i-2) in seq_t. */
static seq_t fibonacci(idx_t n)
{
    seq_t f = 0, g = 1;              /* F(k), F(k+1) for the bits of n read so far */
    for (int b = 63; b >= 0; b--) {
        seq_t f2 = f * ((seq_t)2 * g - f);
        seq_t g2 = f * f + g * g;
        f = f2;
        g = g2;
        if (((uint64_t)n >> b) & 1) {
            seq_t t = f + g;
            f = g;
            g = t;
        }
    }
    return f;
}

#endif
