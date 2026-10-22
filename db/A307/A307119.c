/*
 * A307119  a(1) = 1, for n>1, a(n) = dp(n-1) + dp(n) + dp(n+1), where dp(n)
 *          is the number of divisors of n less than n (A032741).
 * https://oeis.org/A307119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = dp(n-1) + dp(n) + dp(n+1), dp(k) = number of divisors of
 *            k less than k (i.e., dp(k) = d(k) - 1, where d(k) is the total
 *            divisor count). a(1) = 1
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Helper: number of positive divisors of n (including n itself)      */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t cnt = 0;
    uint64_t i;
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            ++cnt;                     /* divisor i */
            if (i != n / i) ++cnt;     /* paired divisor n/i */
        }
    }
    return cnt;
}
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1, but safe */
    if (n == 1) return 1;           /* base case */

    /* compute dp for n-1, n, n+1 */
    seq_t dpnm1 = (seq_t)(divisor_count_u64((uint64_t)(n - 1)) - 1);
    seq_t dpn   = (seq_t)(divisor_count_u64((uint64_t)n)     - 1);
    seq_t dpnp1 = (seq_t)(divisor_count_u64((uint64_t)(n + 1)) - 1);

    return dpnm1 + dpn + dpnp1;
}
