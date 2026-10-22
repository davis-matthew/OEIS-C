/*
 * A340959  Table read by antidiagonals of the smallest prime >= n^k, n >= 1
 *          and k >= 0.
 * https://oeis.org/A340959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the smallest prime ≥ N^K where (N,K) are the coordinates
 *            of the n‑th entry read from the table T(N,K) = next_prime(N^K)
 *            antidiagonally, with each diagonal traversed in decreasing N
 *            order (i.e., largest N first)
 * Range:     u8 n <= 32, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 ≥ n. */
    idx_t d = 1;
    while ( (d * (d + 1)) / 2 < n )
        ++d;

    /* Previous total terms before diagonal d. */
    idx_t prev_total = (d * (d - 1)) / 2;

    /* Zero‑based offset within the diagonal. */
    idx_t offset = n - prev_total - 1;     /* 0 ≤ offset < d */

    /* Coordinates along the diagonal (descending N). */
    seq_t N = (seq_t)(d - offset);        /* N ≥ 1 */
    seq_t K = (seq_t)(offset);            /* K ≥ 0 */

    /* Compute N^K as arbitrary‑precision integer. */
    seq_t power = big_pow(N, K);

    /* Find the smallest prime ≥ power. */
    seq_t candidate = power;
    if (candidate <= (seq_t)1)
        candidate = (seq_t)2;
    while (!is_prime_big(candidate))
        ++candidate;
    return candidate;
}
