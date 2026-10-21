/*
 * A259651  Number of distinct prime factors of the n-th pentagonal number
 *          (A000326).
 * https://oeis.org/A259651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001221(A000326(n)) * A000326(n) = n*(3n-1)/2 is the nth
 *            pentagonal number. a(n) is the number of * distinct prime
 *            factors of that number
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* The built‑in helper only accepts 64‑bit inputs; the problem limits n
     * to values for which the pentagonal number fits in uint64_t (e.g. n ≤ 10⁴). */
    if (n == 0)
        return 0;

    uint64_t nn = n;
    uint64_t pent = nn * (3ULL * nn - 1ULL) / 2ULL;

    int cnt = count_distinct_primes(pent);
    return (seq_t)cnt;
}
