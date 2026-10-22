/*
 * A352100  Number of partitions of 2n into two odd parts that are not both
 *          prime.
 * https://oeis.org/A352100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A352100
    a(n) = ceil(n/2) – (number of unordered pairs of odd primes summing to 2n)

    The number of such pairs is counted by iterating over odd primes p ≤ n
    and checking whether q = 2n – p is prime.  This is a direct O(n √n)
    algorithm, fast enough for n ≤ 10 000 and independent of the build
    utilities.
*/

#include <stdint.h>
#include <stdbool.h>

/* Helper: trial‑division primality test for idx_t values. */
static inline bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1u) == 0) return false;      /* even > 2 */
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* ceil(n/2) in seq_t arithmetic */
    seq_t half = ((seq_t)n + 1u) / 2u;

    /* Count unordered pairs (p, q) of odd primes with p + q = 2n,
       where p ≤ n. */
    seq_t gold = 0;
    idx_t two_n = n + n;          /* 2n */
    for (idx_t p = 3; p <= n; p += 2) {   /* only odd p ≥ 3 */
        if (!is_prime_idx(p))
            continue;
        idx_t q = two_n - p;
        if (q < 3)                      /* q must be odd prime ≥ 3 */
            continue;
        if (is_prime_idx(q))
            ++gold;
    }

    return half - gold;
}
