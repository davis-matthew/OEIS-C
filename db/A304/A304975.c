/*
 * A304975  Number of achiral color patterns (set partitions) for a row or
 *          loop of length n using exactly 5 colors (sets).
 * https://oeis.org/A304975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n%2==0] * (3*S2(n/2+2,5) - 11*S2(n/2+1,5) + 6*S2(n/2,5))
 *            * + [n%2==1] * (S2((n+5)/2,5) - 3*S2((n+3)/2,5)) * where S2(n,k)
 *            is the Stirling number of the second kind
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 57, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* indices needed for Stirling numbers */
    bool even = (n % 2 == 0);
    idx_t m      = n / 2;         /* floor(n/2) */
    idx_t idx_m  = m;             /* S2(m,5)   */
    idx_t idx_m1 = m + 1;         /* S2(m+1,5) */
    idx_t idx_m2 = m + 2;         /* S2(m+2,5) */
    idx_t idx_k1 = (n + 5) / 2;   /* S2((n+5)/2,5) - used when n odd */
    idx_t idx_k2 = (n + 3) / 2;   /* S2((n+3)/2,5) */

    /* maximum n required for S2 computation */
    idx_t needed = idx_m2;
    if (!even && idx_k1 > needed)
        needed = idx_k1;

    /* dp[k] holds S2(current_n, k) for k=0..5 */
    seq_t dp[6];
    dp[0] = 1;
    for (int k = 1; k <= 5; ++k)
        dp[k] = 0;

    /* captured values for required indices */
    seq_t val_m  = 0, val_m1 = 0, val_m2 = 0;
    seq_t val_k1 = 0, val_k2 = 0;

    /* capture index 0 if needed */
    if (idx_m  == 0) val_m  = dp[5];
    if (idx_m1 == 0) val_m1 = dp[5];
    if (idx_m2 == 0) val_m2 = dp[5];
    if (idx_k1 == 0) val_k1 = dp[5];
    if (idx_k2 == 0) val_k2 = dp[5];

    /* compute Stirling numbers up to the largest needed index */
    for (idx_t i = 1; i <= needed; ++i) {
        for (int k = 5; k >= 1; --k)
            dp[k] = dp[k] * (seq_t)k + dp[k - 1];
        dp[0] = 0;  /* S2(i,0)=0 for i>0 */

        if (i == idx_m)  val_m  = dp[5];
        if (i == idx_m1) val_m1 = dp[5];
        if (i == idx_m2) val_m2 = dp[5];
        if (i == idx_k1) val_k1 = dp[5];
        if (i == idx_k2) val_k2 = dp[5];
    }

    /* combine according to parity */
    seq_t result;
    if (even) {
        result = (seq_t)3 * val_m2
               + (seq_t)6 * val_m
               - (seq_t)11 * val_m1;
    } else {
        result = val_k1 - (seq_t)3 * val_k2;
    }

    return result;
}
