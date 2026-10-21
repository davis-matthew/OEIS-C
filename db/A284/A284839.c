/*
 * A284839  Number of compositions (ordered partitions) of n into prime power
 *          divisors of n (including 1).
 * https://oeis.org/A284839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into prime‑power divisors of
 *            n * (including 1). We compute the set of prime‑power divisors of
 *            n, then use * a simple dynamic‑programming routine to count
 *            ordered partitions. * * The implementation only uses integer
 *            arithmetic and the types supplied * by the buil...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 71, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* needed for integer types used in the example build */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;     /* a(0) = 1 by definition */

    /* Collect prime‑power divisors of n (including 1). */
    idx_t parts[64];
    size_t parts_cnt = 0;
    parts[parts_cnt++] = 1;           /* 1 always allowed */

    idx_t temp = n;

    /* Handle factor 2 separately for efficiency. */
    if ((temp & 1) == 0) {
        idx_t power = 1;
        while ((temp & 1) == 0) {
            temp >>= 1;               /* divide by 2 */
            power <<= 1;              /* multiply by 2 */
            if (parts_cnt < 64) parts[parts_cnt++] = power;
        }
    }

    /* Trial division for odd primes. */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t power = 1;
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                if (parts_cnt < 64) parts[parts_cnt++] = power;
            }
        }
    }

    /* Remaining prime factor, if any. */
    if (temp > 1) {
        if (parts_cnt < 64) parts[parts_cnt++] = temp;
    }

    /* Dynamic‑programming array.  Static because n ≤ 10000 for the tests. */
    static seq_t dp[10001];
    dp[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (size_t j = 0; j < parts_cnt; ++j) {
            idx_t d = parts[j];
            if (d <= i) sum += dp[i - d];
        }
        dp[i] = sum;
    }

    return dp[n];
}
