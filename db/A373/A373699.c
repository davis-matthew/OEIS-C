/*
 * A373699  a(n) is the area of the (n+1)-gon having vertices coordinates
 *          (2,0), (2,prime(2)), ..., (n,prime(n)), (n,0).
 * https://oeis.org/A373699
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2}^{n-1} (prime(k)+prime(k+1))/2
 * Range:     u8 n <= 13, u16 n <= 157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 8952 (2026-11-03)
 * Helpers:   build_primes_upto, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = (S(n-1)+S(n)-7)/2 where S(m)=sum_{i=1}^{m} prime(i) */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;        /* out of defined range */

    static uint64_t *primes = NULL;     /* array of primes up to current limit */
    static seq_t  *cumsum = NULL;       /* cumulative sums: cumsum[i] = sum_{j=1}^i prime(j) */
    static size_t prime_cnt = 0;        /* number of primes stored */
    static idx_t  last_n = 0;           /* largest n processed so far */

    /* regenerate primes and cumulative sums when a larger n is requested */
    if (n > last_n) {
        /* compute an upper bound for the nth prime */
        idx_t limit = _prime_limit_for_idx(n);

        size_t cnt = 0;
        uint64_t *new_primes = NULL;
        build_primes_upto(limit, &new_primes, &cnt);

        primes    = new_primes;             /* keep old array until larger request */
        prime_cnt = cnt;
        last_n    = n;

        /* free old cumulative sum array and allocate a new one */
        if (cumsum) seq_free_array(cumsum);
        cumsum = seq_alloc_new_array(prime_cnt + 1);
        cumsum[0] = (seq_t)0;
        for (size_t i = 1; i <= prime_cnt; ++i) {
            seq_t p = (seq_t)primes[i - 1];
            cumsum[i] = cumsum[i - 1] + p;
        }
    }

    /* now prime_cnt >= n, so indices n-1 and n are valid */
    seq_t s1 = cumsum[n - 1];          /* sum of first (n-1) primes  */
    seq_t s2 = cumsum[n];              /* sum of first n primes     */
    seq_t result = (s1 + s2 - (seq_t)7) / (seq_t)2;
    return result;
}
