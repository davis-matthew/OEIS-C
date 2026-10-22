/*
 * A391659  Number of plane partitions of n with 5 parts.
 * https://oeis.org/A391659
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of q^n in
 *            (3*q^11+5*q^9+6*q^8+5*q^7+7*q^5)/((1-q)(1-q^2)(1-q^3)(1-q^4)(1-q^5))
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, free */

seq_t a(idx_t n)
{
    /* Only defined for n ≥ 5; below the offset the behaviour does not matter. */
    if (n < 5)
        return 0;

    /* Allocate array for the partition numbers with parts ≤5. */
    idx_t size = n + 1;                 /* indices 0 … n */
    seq_t *f = (seq_t *)malloc((size_t)size * sizeof(seq_t));
    if (!f)                               /* allocation failed */
        return 0;

    /* Initialise and compute f using the standard coin‑change DP. */
    for (idx_t i = 0; i < size; ++i)
        f[i] = 0;
    f[0] = 1;                            /* one way to make 0 */

    for (idx_t coin = 1; coin <= 5; ++coin) {
        for (idx_t v = coin; v < size; ++v) {
            f[v] += f[v - coin];
        }
    }

    /* Multiply the numerator polynomial by the computed series. */
    seq_t result = 0;
    result += (seq_t)7 * f[n - 5];          /* 7*q^5  */
    if (n >= 7) result += (seq_t)5 * f[n - 7]; /* 5*q^7 */
    if (n >= 8) result += (seq_t)6 * f[n - 8]; /* 6*q^8 */
    if (n >= 9) result += (seq_t)5 * f[n - 9]; /* 5*q^9 */
    if (n >= 11) result += (seq_t)3 * f[n - 11];/* 3*q^11 */

    free(f);
    return result;
}
