/*
 * A309155  For integer n with prime factors p_i (1 <= i <= r), with
 *          repetition, (Omega(n) = r); a(n) = Sum_{i=1..r} k_i, where k_i is
 *          the least positive integer such that p_i - k_i | n - k_i.
 * https://oeis.org/A309155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 513, u16 n <= 8177, u32 n <= 8177, u64 n <= 8177,
 *            u128 n <= 8177, big any n
 * Helpers:   find_min_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A309155: for each prime factor p of n (with multiplicity)
   add the least positive integer k such that (p-k) divides (n-k). */
#include <stdint.h>
#include "../utils/find_min_k.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;         /* a(1) = 0, and a(0) is not used          */
    uint64_t m = n;
    seq_t total = 0;

    /* factor 2 */
    uint64_t exp = 0;
    while ((m & 1ULL) == 0ULL) {
        ++exp;
        m >>= 1;
    }
    if (exp) total += find_min_k(2, n) * exp;

    /* factor odd primes */
    for (uint64_t i = 3; i * i <= m; i += 2) {
        if (m % i == 0) {
            exp = 0;
            while (m % i == 0) {
                ++exp;
                m /= i;
            }
            total += find_min_k(i, n) * exp;
        }
    }

    /* remaining prime > sqrt(m) */
    if (m > 1) {
        total += find_min_k(m, n);
    }

    return total;
}
