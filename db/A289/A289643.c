/*
 * A289643  a(n) = n*(2*n+1)*binomial(n+2,n)/3.
 * https://oeis.org/A289643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n+1)*binomial(n+2,n)/3
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for sanity, not required for the build */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Values that will be multiplied */
    seq_t a1 = (seq_t)n;
    seq_t a2 = (seq_t)(n + 1);
    seq_t a3 = (seq_t)(n + 2);
    seq_t a4 = (seq_t)(2 * n + 1);

    /* Reduce by 2 */
    if ((a1 % 2) == 0)
        a1 /= 2;
    else if ((a2 % 2) == 0)
        a2 /= 2;
    else                      /* must divide a3 */
        a3 /= 2;

    /* Reduce by 3 */
    if ((a1 % 3) == 0)
        a1 /= 3;
    else if ((a2 % 3) == 0)
        a2 /= 3;
    else if ((a3 % 3) == 0)
        a3 /= 3;
    else                      /* must divide a4 */
        a4 /= 3;

    /* Final product */
    seq_t result = a1 * a2;
    result *= a3;
    result *= a4;

    return result;
}
