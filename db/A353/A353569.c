/*
 * A353569  a(n) = 1 if n is an odd number divisible by a square > 1,
 *          otherwise 0.
 * https://oeis.org/A353569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and divisible by a square > 1, otherwise 0
 * Range:     u8 n <= 8266, u16 n <= 8266, u32 n <= 8266, u64 n <= 8266,
 *            u128 n <= 8266, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for 0 <= n < 2^64 */
static idx_t floor_isqrt(idx_t n)
{
    if (n == 0) return 0;
    idx_t low  = 1;
    idx_t high = (idx_t)4294967296ULL;           /* 2^32, upper bound for sqrt(2^64-1) */

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        /* division by mid is safe because mid >= 1 */
        if (mid <= n / mid)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* even numbers can never satisfy the condition */
    if ((n & 1ULL) == 0) return 0;

    idx_t limit = floor_isqrt(n);
    for (idx_t p = 3; p <= limit; p += 2) {
        idx_t sq = p * p;
        if (n % sq == 0) return 1;
    }
    return 0;
}
