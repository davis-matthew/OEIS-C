/*
 * A326504  Number of (binary) max-heaps on n elements from the set {0,1}
 *          containing exactly three 0's.
 * https://oeis.org/A326504
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)*(n^2-5*n+39)/48 + (n mod 2)*(n^2-11*n+16)/16
 *            +((n mod 4) - ((n+1) mod 4))/4)
 * Range:     u8 n <= 24, u16 n <= 148, u32 n <= 5908, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t
a(idx_t n)
{
    /* All intermediate math uses signed 64‑bit to avoid overflow and
       handle potential negative sub‑terms before adding them together. */
    int64_t nt = (int64_t)n;
    int64_t n2 = nt * nt;

    int64_t termA = (nt - 1) * (n2 - 5 * nt + 39);

    int64_t partB = (nt % 2) * (n2 - 11 * nt + 16);
    partB *= 3;                     /* multiply numerator by 3 for denominator 48 */

    int64_t mod4      = nt % 4;
    int64_t mod4next  = (nt + 1) % 4;
    int64_t partC = (mod4 - mod4next) * 12;  /* 12*(n%4 - (n+1)%4) */

    int64_t total = termA + partB + partC;

    /* For n >= 3 the total is non‑negative; we take floor division by 48. */
    return (seq_t)(total / 48);
}
