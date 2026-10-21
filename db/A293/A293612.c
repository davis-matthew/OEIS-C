/*
 * A293612  a(n) = (1/2)*(n + 1)*(5*n^2 + 15*n + 6)*Pochhammer(n, 6) / 6!.
 * https://oeis.org/A293612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n*(1+n)^2*(720 + 2724*n + 3336*n^2 + 1919*n^3 + 571*n^4
 *            + 85*n^5 + 5*n^6))) / 1440 which simplifies to a(n) =
 *            (1/2)*(n+1)*(5*n^2+15*n+6)*Pochhammer(n,6)/6!
 *            (Pochhammer(n,6)=n*(n+1)*...*(n+5))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 20, u64 n <= 257, u128 n <= 1000,
 *            big any n
 * Helpers:   divide_common
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/divide_common.h"

seq_t a(idx_t n)
{
    const uint64_t DENOM = 1440ULL;                /* 2^6 * 3^2 *5   */

    seq_t nn = (seq_t)n;                          /* cast the index */

    /* Build the list of multiplicative factors:
         n, n+1, n+2, n+3, n+4, n+5, (n+1), (5*n^2+15*n+6)   */
    const size_t NF = 8;
    seq_t factors[NF];

    factors[0] = nn;
    factors[1] = nn + 1;
    factors[2] = nn + 2;
    factors[3] = nn + 3;
    factors[4] = nn + 4;
    factors[5] = nn + 5;
    factors[6] = nn + 1;                          /* duplicate (n+1) */
    /* polynomial 5*n^2 + 15*n + 6 */
    factors[7] = (5 * nn * nn) + (15 * nn) + 6;

    seq_t result = 1;
    uint64_t rem = DENOM;

    /* Iterate over each factor, reducing before and after multiplication
       to keep intermediate values small. */
    for (size_t i = 0; i < NF; ++i) {
        seq_t f = factors[i];

        /* Reduce f with the remaining part of the denominator */
        divide_common(&f, &rem, 2, 6);
        divide_common(&f, &rem, 3, 2);
        divide_common(&f, &rem, 5, 1);

        /* Multiply */
        result *= f;

        /* Reduce the product with the denominator as much as possible */
        divide_common(&result, &rem, 2, 6);
        divide_common(&result, &rem, 3, 2);
        divide_common(&result, &rem, 5, 1);
    }

    /* Any divisor left (should be 1) is removed */
    if (rem > 1) {
        result /= rem;
    }

    return result;
}
