/*
 * A275235  Number of primes between n and n+log(n)^2.
 * https://oeis.org/A275235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes p with n < p <= n + (log n)^2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

static uint64_t *global_primes = NULL; /* cached list of primes up to global_limit */
static size_t       global_cnt   = 0;   /* number of primes stored */
static idx_t        global_limit = 0;   /* maximal n for which primes are cached */

/* binary search: first index with value > limit */
static size_t upper_bound(const uint64_t *pr, size_t cnt, uint64_t limit)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (pr[mid] <= limit) lo = mid + 1;
        else                  hi = mid;
    }
    return lo;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* n is at least 1 by the problem description; we ignore lower indices. */
    double nd = (double)n;
    double ln  = log(nd);                     /* natural logarithm */
    double u_d = nd + ln * ln;                /* n + (log n)^2 */
    uint64_t upper = (uint64_t)floor(u_d + 1e-12);

    /* extend prime cache if necessary */
    if (upper > global_limit) {
        free(global_primes);
        build_primes_upto(upper, &global_primes, &global_cnt);
        global_limit = upper;
    }

    /* count primes in the open interval (n , upper] */
    size_t idx_n = upper_bound(global_primes, global_cnt, (uint64_t)n);
    size_t idx_u = upper_bound(global_primes, global_cnt, upper);
    return (seq_t)(idx_u - idx_n);
}
