/*
 * A376567  a(n) = binomial(bigomega(n) + omega(n), omega(n)), where bigomega
 *          = A001222 and omega = A001221.
 * https://oeis.org/A376567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(bigomega(n) + omega(n), omega(n))
 * Range:     u8 n <= 1679, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, binomial, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where bigomega(n) is A001222 (total prime factors with multiplicity)  */
/* and omega(n)    is A001221 (number of distinct prime factors).      */
#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of OEIS offset, unused  */
    /* total number of prime factors with multiplicity   */
    seq_t bigo = bigomega(n);            /* A001222(n) */
    /* number of distinct prime factors               */
    idx_t om   = (idx_t)count_distinct_primes((uint64_t)n); /* A001221(n) */
    /* sum bigo + om fits easily into idx_t for all n */
    idx_t tot  = (idx_t)bigo + om;
    return binomial(tot, om);            /* A010846(tot, om) */
}
