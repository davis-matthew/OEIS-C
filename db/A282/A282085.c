/*
 * A282085  Number of n-element subsets of [n+9] having an even sum.
 * https://oeis.org/A282085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i even} binomial( ceil((n+9)/2), i ) * binomial(
 *            floor((n+9)/2), n-i )
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 47, u64 n <= 614,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of n‑element subsets of [n+9] having an even sum. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t m = n + 9;                     /* size of the set [1 .. m] */
    idx_t O = (m + 1) / 2;               /* number of odd elements */
    idx_t E = m / 2;                     /* number of even elements */

    seq_t sum = 0;
    for (idx_t i = 0; i <= n && i <= O; i += 2) {
        seq_t t1 = binomial(O, i);        /* choose i odds */
        seq_t t2 = binomial(E, n - i);    /* choose remaining evens */
        sum += t1 * t2;                   /* add contribution */
    }
    return sum;
}
