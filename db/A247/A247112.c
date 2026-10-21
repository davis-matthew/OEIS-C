/*
 * A247112  Floor of sums of the cubes of the non-integer square roots of n,
 *          as partitioned by the integer roots:
 *          floor(Sum_{j=n^2+1..(n+1)^2-1} j^(3/2)).
 * https://oeis.org/A247112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n^3 + 6*n^2 + 5*n + 1)/2
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 214, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow on 64‑bit builds. */
    __int128 nn = n;
    __int128 n2 = nn * nn;            /* n^2 */
    __int128 n3 = n2 * nn;            /* n^3 */
    __int128 term = 4 * n3 + 6 * n2 + 5 * nn + 1; /* 4n^3+6n^2+5n+1 */

    __int128 prod;
    if ((n & 1ULL) == 0) {
        /* n even: divide n by 2 */
        prod = (nn / 2) * term;
    } else {
        /* n odd: term is even, divide it by 2 */
        prod = nn * (term / 2);
    }

    return (seq_t)prod;
}
