/*
 * A354841  Number of primes between n and n+log(n), exclusive.
 * https://oeis.org/A354841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{p prime | n < p < n + log(n)}
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* static cache of primes up to the largest bound seen so far */
    static uint64_t *primes = NULL;   /* array of primes */
    static size_t  prime_cnt  = 0;    /* number of primes in the array */
    static idx_t   max_limit  = 0;    /* largest limit for which primes[] is valid */

    if (n < 1)
        return (seq_t)0;

    /* Compute upper bound of the interval: floor(n + log(n) - ε)  */
    double limit = (double)n + log((double)n);
    idx_t bound = (idx_t)floor(limit - 1e-12);      /* exclude integer upper bound */

    if (bound < 2 || bound <= n)
        return (seq_t)0;

    /* Rebuild cache if needed */
    if (bound > max_limit) {
        if (primes != NULL) free(primes);
        build_primes_upto(bound, &primes, &prime_cnt);
        max_limit = bound;
    }

    /* Find first prime > n */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] <= (uint64_t)n)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t start_idx = lo;

    /* Find first prime > bound */
    lo = 0; hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] <= (uint64_t)bound)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t end_idx = lo;

    seq_t cnt = (seq_t)(end_idx > start_idx ? (end_idx - start_idx) : 0);
    return cnt;
}
