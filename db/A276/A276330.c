/*
 * A276330  a(n) = largest term of A001563 that divides n, a(0) = 0.
 * https://oeis.org/A276330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = largest k*k! (k >= 1) dividing n, found by
 *            trial over the terms of A001563
 * Range:     u8 n <= 599, u16 n <= 4319, u32 n <= 4319, u64 n <= 4319,
 *            u128 n <= 4319, big any n
 * Validated: standard, pass, 113 terms through n = 4169 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 fact = 1;   /* k! */
    unsigned __int128 best = 1;
    for (unsigned k = 1; ; k++) {
        fact *= k;
        unsigned __int128 term = fact * k;
        if (term > N) break;
        if (N % term == 0) best = term;
    }
    return (seq_t)best;
}
