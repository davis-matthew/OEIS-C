/*
 * A339353  G.f.: Sum_{k>=1} k^2 * x^(k*(k + 1)) / (1 - x^k).
 * https://oeis.org/A339353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sum of squares of divisors of n that are smaller than sqrt(n)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t isqrt(idx_t x)
{
    /* Computes floor(sqrt(x)) using only integer arithmetic. */
    idx_t lo = 0;
    idx_t hi = (x >> 1) + 1;   /* sqrt(x) <= x/2 + 1 for x >= 0 */

    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;   /* upper mid to avoid infinite loop */
        if (mid != 0 && mid > x / mid)     /* mid*mid > x avoiding overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0 but guard against underflow */

    seq_t sum = 0;
    idx_t limit = isqrt(n - 1);              /* floor(sqrt(n-1))  ensures d^2 < n */

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            seq_t v = (seq_t)d;
            sum += v * v;                    /* add d^2 to the sum */
        }
    }
    return sum;
}
