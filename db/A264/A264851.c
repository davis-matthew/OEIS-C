/*
 * A264851  a(n) = n*(n + 1)*(n + 2)*(4*n - 3)/6.
 * https://oeis.org/A264851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(4*n-3)/6
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 282, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Direct polynomial evaluation with a 128‑bit intermediate to
       avoid overflow when seq_t is 64‑bit.  For arbitrary‑precision
       builds the cast to seq_t truncates correctly if the value
       fits, otherwise the seq_t type will hold the full magnitude. */
    if (n == 0) return (seq_t)0;
    unsigned __int128 prod = (unsigned __int128)n * (n + 1);
    prod *= (n + 2);
    prod *= (4 * n - 3);
    prod /= 6;
    return (seq_t)prod;
}
