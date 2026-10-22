/*
 * A373895  a(n) = ceiling((2^n+n-1)/n).
 * https://oeis.org/A373895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((2^n + n - 1) / n)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* For n <= 70 the result fits in 64 bits.  For larger n the
   sequence term no longer fits in a 64‑bit unsigned integer, so
   the extra cases are only required for the arbitrary‑precision
   build.  The generic implementation below works for any seq_t. */
seq_t a(idx_t n)
{
    if (n == 0) {             /* not used (offset starts at 1) */
        return (seq_t)0;
    }

    /* --- small n: use 128‑bit arithmetic to avoid overflow --- */
    /* 2^70 = 1.18e21 < 2^128, so shifts up to n = 70 are safe. */
    if (n <= 70) {
        unsigned __int128 q = ((unsigned __int128)1 << n);   /* 2^n */
        unsigned __int128 num = q + (unsigned __int128)n - 1ULL;
        unsigned __int128 div = (num + (unsigned __int128)n - 1ULL) / (unsigned __int128)n;
        return (seq_t)div;
    }

    /* --- general case: use seq_t operations (works for arbitrary‑precision) --- */
    /* Build 2^n by repeated left shift */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        power <<= 1;                     /* 2 * power                         */
    }

    seq_t numerator = power + (seq_t)n - 1;
    seq_t result = numerator / (seq_t)n;  /* floor division                     */
    if (numerator % (seq_t)n != 0) {       /* add one if not divisible           */
        ++result;
    }
    return result;
}
