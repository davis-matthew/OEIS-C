/*
 * A327642  Number of partitions of n into divisors of n that are at most
 *          sqrt(n).
 * https://oeis.org/A327642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 95, u32 n <= 335, u64 n <= 1259,
 *            u128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A327642: Number of partitions of n into divisors of n that are ≤ √n
   Formula: a(n) = [xⁿ] ∏_{d|n, d≤√n} 1/(1−xᵈ)
   The implementation follows the coin‑change formulation of this product. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root (floor) for 64‑bit unsigned values.
   Works for all n ≤ 2⁶⁴−1 without overflow. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n + 1;   /* hi > sqrt(n) */
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > n / mid)
            hi = mid;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n==0. */
    if (n == 0) return (seq_t)1;

    /* Find all divisors of n that are ≤ √n.  For n ≤ 10 000 this never
       exceeds 128 candidates, which is enough for the test sets.  */
    idx_t coins[128];
    size_t coin_cnt = 0;
    idx_t sqrt_n = floor_isqrt_u64(n);
    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            /* i is a divisor ≤ √n.  The complementary divisor n/i is
               ≥ √n and is not considered. */
            coins[coin_cnt++] = i;
        }
    }

    /* Dynamic programming array for the coin‑change counts.
       Size is n+1.  For the intended n (≤ 10 000) this is trivial. */
    size_t dp_size = (size_t)(n + 1);
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp) return (seq_t)0;      /* allocation failure – unlikely */

    dp[0] = (seq_t)1;              /* there is exactly one way to make 0 */

    /* For each allowed divisor d work out all ways to create sums
       up to n using unlimited copies of d (order doesn't matter). */
    for (size_t ci = 0; ci < coin_cnt; ++ci) {
        idx_t d = coins[ci];
        for (idx_t s = d; s <= n; ++s) {
            dp[s] += dp[s - d];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
