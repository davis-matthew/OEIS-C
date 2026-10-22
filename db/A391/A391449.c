/*
 * A391449  a(n) = A252489(n) - A059957(n).
 * https://oeis.org/A391449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(gpf(n*(n+1))) - count_distinct_primes(n*(n+1))
 * Range:     u8 n <= 1635, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, count_distinct_primes, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_pi.h"
#include "../utils/gpf.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    uint64_t nn = (uint64_t)n;
    uint64_t prod = nn * (nn + 1);        /* product fits in 64 bits for the required range */
    uint64_t largest = gpf(prod);          /* greatest prime factor of n*(n+1) */
    seq_t index = compute_pi((idx_t)largest);   /* index of that prime */
    seq_t distinct = (seq_t)count_distinct_primes(prod); /* number of distinct prime factors */
    return index - distinct;
}
