/*
 * A373937  Number of compositions of 7*n-5 into parts 6 and 7.
 * https://oeis.org/A373937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor((n-5)/6)} binomial(n+k, n-5-6*k)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of compositions of 7*n-5 into parts 6 and 7. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5)                      /* n=1..4 give 0 automatically */
        return 0;

    seq_t res = 0;
    idx_t maxk = (n - 5) / 6;        /* largest k such that n-5-6*k >= 0 */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t bottom = n - 5 - 6 * k;
        idx_t top    = n + k;
        res += binomial(top, bottom);
    }
    return res;
}
