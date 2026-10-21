/*
 * A281259  Number of elements of S_n with strategic pile of size 6.
 * https://oeis.org/A281259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-6)!*(120*binomial(n-7,5) + 576*binomial(n-7,4) * +
 *            1116*binomial(n-7,3) + 1080*binomial(n-7,2) * +
 *            540*binomial(n-7,1)) for n>7 * a(n) = 0 for n <= 7
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n <= 7) {
        return 0;
    }

    idx_t m = n - 7;                      /* m = n-7 ≥ 1 */

    seq_t sum =
        (seq_t)120 * binomial(m, 5) +
        (seq_t)576 * binomial(m, 4) +
        (seq_t)1116 * binomial(m, 3) +
        (seq_t)1080 * binomial(m, 2) +
        (seq_t)540  * binomial(m, 1);

    seq_t fact = factorial(n - 6);        /* (n‑6)! */

    return fact * sum;
}
