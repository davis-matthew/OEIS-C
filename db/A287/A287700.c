/*
 * A287700  a(n) = (4!)^3 * [z^4] hypergeom([], [1,1], z)^n.
 * https://oeis.org/A287700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -1899*n + 3916*n^2 - 2592*n^3 + 576*n^4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute all intermediates as signed 64‑bit integers,
       which are sufficient for n up to 10 000 (see analysis). */
    int64_t n64 = (int64_t)n;
    int64_t n2  = n64 * n64;
    int64_t n3  = n2  * n64;
    int64_t n4  = n3  * n64;

    /* quartic polynomial in a compact form */
    int64_t result = 576LL * n4
                   - 2592LL * n3
                   + 3916LL * n2
                   - 1899LL * n64;

    return (seq_t)result;
}
