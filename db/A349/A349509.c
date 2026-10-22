/*
 * A349509  a(n) is the denominator of binomial(n^3 + 6*n^2 - 6*n + 2, n^3 -
 *          1)/n^3.
 * https://oeis.org/A349509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1 if for some digit position i the sum of the *
 *            corresponding base‑3 digits of n³ and (6n²−6n+3) is at least 3;
 *            * otherwise a(n)=3. The special case a(1)=1 is handled
 *            separately
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Compute n³ and 6n²−6n+3 using 128‑bit arithmetic to avoid overflow
     * for all n that comfortably fit in the target sys. */
    unsigned __int128 n128 = (unsigned __int128)(uint64_t)n;
    unsigned __int128 x = n128 * n128 * n128;             /* n³          */
    unsigned __int128 y = (unsigned __int128)6 * n128 * n128
                         - (unsigned __int128)6 * n128
                         + (unsigned __int128)3;       /* 6n²−6n+3   */

    /* Scan the base‑3 digits from least significant to most significant. */
    while (x || y)
    {
        unsigned long long d1 = (unsigned long long)(x % 3);
        unsigned long long d2 = (unsigned long long)(y % 3);
        if (d1 + d2 >= 3)
            return (seq_t)1;
        x /= 3;
        y /= 3;
    }

    return (seq_t)3;
}
