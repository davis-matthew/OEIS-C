/*
 * A259455  n Sum_n Sum_n Sum_n.
 * https://oeis.org/A259455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3*(n+3)*(n+2)*(n+1)^2/48
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 40, u64 n <= 981, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t if needed, but idx_t suffices */

seq_t a(idx_t n)
{
    /* n >= 1 by definition; for n==0 we may return 0 but is unused */
    if (n == 0) return 0;

    /* Arrange the seven factors of the numerator */
    seq_t f[7];
    f[0] = (seq_t)n;
    f[1] = (seq_t)n;
    f[2] = (seq_t)n;
    f[3] = (seq_t)n + 1;
    f[4] = (seq_t)n + 1;
    f[5] = (seq_t)n + 2;
    f[6] = (seq_t)n + 3;

    /* The denominator is 48 = 2^4 * 3 */
    int denom = 48;

    /* Cancel the factors 48 from the numerator factors one at a time */
    for (idx_t i = 0; i < 7; ++i) {
        while ((denom % 2 == 0) && ((f[i] % 2) == 0)) {
            f[i] /= 2;
            denom /= 2;
        }
        while ((denom % 3 == 0) && ((f[i] % 3) == 0)) {
            f[i] /= 3;
            denom /= 3;
        }
    }

    /* After the loop, denom should be 1 */
    (void)denom;  /* silence unused variable warning */

    /* Multiply the remaining factors; all intermediates fit in seq_t */
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < 7; ++i)
        prod *= f[i];

    return prod;
}
