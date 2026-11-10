/*
 * A258170  T(n,k) = (1/k!) * Sum_{i=0..k} (-1)^(k-i) * C(k,i) * A185651(n,i);
 *          triangle T(n,k), n >= 0, 0 <= k <= n, read by rows.
 * https://oeis.org/A258170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{d|n} phi(d)*Stirling2(n/d,k) for n>=1 (equivalent
 *            to 1/k!*Sum_{i=0..k} (-1)^(k-i)*C(k,i)*A185651(n,i)); T(0,0)=0;
 *            triangle read by rows
 * Range:     u8 n <= 30, u16 n <= 69, u32 n <= 157, u64 n <= 385,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 82 terms through n = 9569 (2026-11-07)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    idx_t k = n - (r * (r + 1)) / 2;

    if (r == 0 || k == 0)
        return 0;

    seq_t *dp = mem_new(seq_t, (size_t)k + 1);
    dp[0] = 1;
    seq_t sum = 0;
    for (idx_t m = 1; m <= r; ++m) {
        for (idx_t j = k; j >= 1; --j)
            dp[j] = dp[j] * (seq_t)j + dp[j - 1];
        dp[0] = 0;
        if (r % m == 0)
            sum += totient(r / m) * dp[k];
    }
    mem_free(dp);
    return sum;
}
