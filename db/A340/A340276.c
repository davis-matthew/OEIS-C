/*
 * A340276  Number of partitions of n into 3 parts whose largest two parts are
 *          relatively prime.
 * https://oeis.org/A340276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/3)} Sum_{i=k..floor((n-k)/2)}
 *            floor(1/gcd(i, n-i-k))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Explanation: each term counts partitions n = k + i + j with k ≤ i ≤ j and gcd(i, j) = 1 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t count = 0;
    idx_t kmax = n / 3;                  /* k runs from 1 to floor(n/3) */
    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t rem = n - k;               /* remaining sum for i + j */
        idx_t imax = rem / 2;            /* i ≤ j ⇒ i ≤ floor(rem/2) */
        for (idx_t i = k; i <= imax; ++i) {
            idx_t j = rem - i;           /* j = n - k - i */
            seq_t g = gcd((seq_t)i, (seq_t)j);
            if (g == 1) ++count;         /* count only when i and j are coprime */
        }
    }
    return count;
}
