/*
 * A337198  Number of distinct prime factors in A337194(n) =
 *          1+A000265(sigma(n)), where A000265(k) gives the odd part of k.
 * https://oeis.org/A337198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of (1 +
 *            odd_part(sigma(n)))
 * Range:     u8 n <= 984, u16 n <= 9797, u32 n <= 9797, u64 n <= 9797,
 *            u128 n <= 9797, big any n
 * Helpers:   count_distinct_primes, odd_part, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is A337198. */

#include "../utils/sigma_of.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    /* Compute sigma(n) */
    seq_t s = sigma_of(n);
    /* Compute odd part of sigma(n) */
    seq_t odd_s = odd_part(s);
    /* Compute t = 1 + odd_part(sigma(n)) */
    seq_t t = odd_s + 1;
    /* Count distinct prime factors of t (t fits in 64 bits for the intended range) */
    uint64_t t64 = (uint64_t)t;
    seq_t res = (seq_t)count_distinct_primes(t64);
    return res;
}
