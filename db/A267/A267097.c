/*
 * A267097  a(n) = number of 4k+1 primes among first n primes; least monotonic
 *          left inverse of A080147.
 * https://oeis.org/A267097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4k+1 primes among the first n primes (least
 *            monotonic left inverse of A080147)
 * Range:     u8 n <= 520, u16 n <= 9502, u32 n <= 9502, u64 n <= 9502,
 *            u128 n <= 9502, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

seq_t
a(idx_t n)
{
    /* a(1) = 0, a(2) = 0, …  */
    if (n == 0) /* Undefined, but return 0 for safety */
        return (seq_t)0;

    /* Estimate an upper bound for the n-th prime.
       For n >= 6: p_n < n (log n + log log n).                   */
    idx_t limit;
    if (n < 6) {
        limit = 15;   /* safe bound for first few primes */
    } else {
        double dn = (double)n;
        double lnd = log(dn);
        double lnln = log(lnd);
        limit = (idx_t)(dn * (lnd + lnln) + 3.0);
    }

    /* Collect the first n primes (or more, up to 'limit') */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    /* In the unlikely event the bound was too small, enlarge once */
    if (prime_cnt < (size_t)n) {
        free(primes);
        limit = (idx_t)(limit * 1.5) + 10;
        build_primes_upto(limit, &primes, &prime_cnt);
    }

    /* Count primes == 1 mod 4 among the first n primes */
    seq_t count = 0;
    size_t limit_n = (size_t)n;
    for (size_t i = 0; i < limit_n; ++i) {
        if ((primes[i] & 3ULL) == 1ULL)  /* primes[i] % 4 == 1 */
            ++count;
    }

    free(primes);
    return count;
}
