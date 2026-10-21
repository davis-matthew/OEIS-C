/*
 * A242856  Number of 2-matchings of the n X n grid graph.
 * https://oeis.org/A242856
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^4 - 4*n^3 - 5*n^2 + 13*n - 4
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 215, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute in 64‑bit unsigned to avoid intermediate overflow.
       The final result is cast to seq_t, which may be 8,16,32,64,128
       bits or an arbitrary‑precision type.  For smaller seq_t the
       value fits and is correctly truncated. */
    uint64_t nn = (uint64_t)n;
    uint64_t n2 = nn * nn;            /* n^2 */
    uint64_t n3 = n2 * nn;            /* n^3 */
    uint64_t n4 = n3 * nn;            /* n^4 */
    uint64_t res = 2 * n4 - 4 * n3 - 5 * n2 + 13 * nn - 4;
    return (seq_t)res;
}
