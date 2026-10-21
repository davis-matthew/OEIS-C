/*
 * A293990  a(n) = (3*n + ((n-2) mod 4))/2.
 * https://oeis.org/A293990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n + ((n-2) mod 4))/2
 * Range:     u8 n <= 170, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n/4 (integer division) and n % 4 */
    idx_t k = n / 4;
    idx_t r = n % 4;

    /* Use the closed‑form piecewise expression to avoid overflow
       on small seq_t types */
    switch (r) {
        case 0: return (seq_t)k * 6 + 1;          /* a(4k)   = 6k+1   */
        case 1: return (seq_t)k * 6 + 3;          /* a(4k+1) = 6k+3   */
        case 2: return (seq_t)k * 6 + 3;          /* a(4k+2) = 6k+3   */
        case 3: return (seq_t)k * 6 + 5;          /* a(4k+3) = 6k+5   */
        default: return 0; /* unreachable */
    }
}
