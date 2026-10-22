/*
 * A389639  Number of ways to write 2*n as the sum of four (not necessarily
 *          distinct) squares of primes.
 * https://oeis.org/A389639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write 2*n as the sum of four (not
 *            necessarily distinct) squares of primes, counted up to order
 *            (i.e., quadruples with i<=j<=k<=l)
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* integer square root for uint64_t values */
static idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* target value 2*n */
    seq_t target = (seq_t)n + (seq_t)n;

    /* upper bound for primes needed (sqrt(2*n)) */
    idx_t limit = isqrt_u64((idx_t)target);

    /* generate all primes <= limit */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t count = 0;

    for (idx_t ia = 0; ia < prime_cnt; ++ia) {
        seq_t sq1 = (seq_t)primes[ia] * primes[ia];
        if (sq1 > target) break;
        for (idx_t ib = ia; ib < prime_cnt; ++ib) {
            seq_t sq2 = (seq_t)primes[ib] * primes[ib];
            seq_t ab = sq1 + sq2;
            if (ab > target) break;
            for (idx_t ic = ib; ic < prime_cnt; ++ic) {
                seq_t sq3 = (seq_t)primes[ic] * primes[ic];
                seq_t abc = ab + sq3;
                if (abc > target) break;
                for (idx_t id = ic; id < prime_cnt; ++id) {
                    seq_t sum = abc + (seq_t)primes[id] * primes[id];
                    if (sum == target)
                        ++count;
                    else if (sum > target)
                        break;
                }
            }
        }
    }

    free(primes);
    return count;
}
