/*
 * A292257  a(n) is the total number of 1's in binary expansion of all proper
 *          divisors of n.
 * https://oeis.org/A292257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n, d<n} popcount(d)
 * Range:     u8 n <= 9517, u16 n <= 9517, u32 n <= 9517, u64 n <= 9517,
 *            u128 n <= 9517, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    seq_t total = 0;
    for (idx_t d = 1; (unsigned __int128)d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t other = n / d;
            if (d < n) {
                total += (seq_t)__builtin_popcountll((unsigned long long)d);
            }
            if (other != d && other < n) {
                total += (seq_t)__builtin_popcountll((unsigned long long)other);
            }
        }
    }
    return total;
}
