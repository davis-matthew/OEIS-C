/*
 * A386476  Number of divisors d of n such that d^2 - 1 is relatively prime to
 *          n.
 * https://oeis.org/A386476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that gcd(d^2 - 1, n) = 1
 * Range:     u8 n <= 340, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    /* Convert n to seq_t once */
    seq_t N = (seq_t)n;

    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            /* Compute (d^2 - 1) modulo N using 128‑bit multiplication to avoid overflow */
            unsigned __int128 sq = (unsigned __int128)d * d;          /* d^2 */
            sq = (sq + (unsigned __int128)N - 1) % (unsigned __int128)N; /* (d^2 - 1) mod N */
            seq_t val = (seq_t)sq;

            if (gcd(val, N) == (seq_t)1)
                ++count;
        }
    }
    return count;
}
