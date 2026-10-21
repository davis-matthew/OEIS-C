/*
 * A271860  a(n) = -Sum_{i=1..n} (-1)^floor(n/i).
 * https://oeis.org/A271860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -Sum_{i=1..n} (-1)^floor(n/i)
 * Range:     u8 n <= 636, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute S(n) = sum_{i=1..n} (-1)^floor(n/i) via
       grouping of equal floor(n/i) values, which takes O(sqrt(n)). */
    int64_t sum = 0;
    idx_t i = 1;
    while (i <= n) {
        idx_t q = n / i;          /* floor(n/i) */
        idx_t last = n / q;       /* largest i with same q */
        idx_t cnt = last - i + 1; /* how many i's produce this q */
        if (q & 1ull)            /* odd -> sign = -1 */
            sum -= (int64_t)cnt;
        else                      /* even -> sign = +1 */
            sum += (int64_t)cnt;
        i = last + 1;
    }
    /* a(n) = -S(n); the result is non‑negative and fits in seq_t */
    return (seq_t)(-sum);
}
