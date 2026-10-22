/*
 * A329752  a(0) = 0, a(n) = a(floor(n/2)) + (n mod 2) * floor(log_2(2n))^2
 *          for n > 0.
 * https://oeis.org/A329752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(n/2) + (n mod 2) * floor(log_2(2n))^2 Recurrence
 *            implementation: a(n)=sum_{i: bit i of n is 1} (m+1-i)^2, where
 *            m=floor(log_2(n))
 * Range:     u8 n <= 286, u16 n <= 8492, u32 n <= 8492, u64 n <= 8492,
 *            u128 n <= 8492, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute m = floor(log2(n)). For non‑zero n this is the index
       of the most significant bit. */
    idx_t m;
#if defined(__GNUC__) || defined(__clang__)
    m = 63 - __builtin_clzll((unsigned long long)n);
#else
    /* Fallback for environments without __builtin_clzll. */
    m = 0;
    idx_t tmp = n;
    while (tmp >>= 1) ++m;
#endif

    seq_t sum = 0;
    for (idx_t i = 0; i <= m; ++i) {
        if ((n >> i) & 1) {
            seq_t t = (seq_t)(m + 1 - i);
            sum += t * t;
        }
    }
    return sum;
}
