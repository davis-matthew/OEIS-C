/*
 * A388200  Array read by antidiagonals: T(n,k) is the number of length n
 *          binary palindromic words in which 1's occur in blocks of at least
 *          k, n >= 0, k >= 1.
 * https://oeis.org/A388200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n',k'), where (n',k') are the antidiagonal coordinates
 *            obtained from n (see OEIS entry)
 * Range:     u8 n <= 134, u16 n <= 526, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes A388200 by extracting the (n',k') pair from the
   integer n, then evaluates T(n',k') using the recurrence
   T(m,k) = 1  for 0 <= m < k,
   T(k,k) = 2,
   T(m,k) = T(m-2,k) + T(m-(k+1),k)  for m > k. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the diagonal d such that
       sum_{i=0}^{d-1} (i+1) <= n < sum_{i=0}^{d} (i+1)  */
    idx_t d = 0;
    idx_t cum = 0;
    while (cum + d + 1 <= n) {
        cum += d + 1;
        d += 1;
    }

    /* Position inside the diagonal (0‑based) */
    idx_t offset = n - cum;
    idx_t i = offset;            /* length */
    idx_t k = d + 1 - offset;    /* minimum block length */

    /* Quick cases */
    if (i < k) return (seq_t)1;      /* only the all‑zero word */
    if (i == k) return (seq_t)2;     /* all‑zero and all‑ones words */

    /* DP table up to i.  i <= 200 for n <= 10000, so stack array is safe. */
    seq_t dp[256] = {0};

    dp[0] = (seq_t)1;
    for (idx_t t = 1; t < k; ++t)
        dp[t] = (seq_t)1;
    dp[k] = (seq_t)2;

    for (idx_t t = k + 1; t <= i; ++t)
        dp[t] = dp[t - 2] + dp[t - (k + 1)];

    return dp[i];
}
