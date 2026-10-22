/*
 * A319007  Sum of the next n nonnegative integers repeated (A004526).
 * https://oeis.org/A319007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(n*n - 2) + (1 - (-1)^n)*(-1)^((n-1)/2)) / 8
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 1, ...  (offset 1) */
    if (n == 0)
        return 0;        /* undefined for n<1, but safe fallback */

    /* Use signed 64‑bit intermediates; for n ≤ 10 000 the values fit easily. */
    int64_t nn = (int64_t)n;
    int64_t term1 = 2LL * nn * (nn * nn - 2LL);   /* 2*n*(n^2 - 2) */

    int64_t term2 = 0;
    if (n & 1ULL) {                      /* odd n */
        int k = (int)((n - 1ULL) / 2ULL); /* (n-1)/2 */
        int sign = (k & 1) ? -1 : 1;      /* (-1)^k */
        term2 = 2LL * sign;              /* 2 * (-1)^k */
    }

    int64_t num = term1 + term2;          /* numerator of the formula */
    return (seq_t)(num / 8LL);             /* divide by 8, truncate towards 0 */
}
