/*
 * A373643  Number of k between consecutive primes such that k is neither
 *          squarefree nor prime powers.
 * https://oeis.org/A373643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Count of k between consecutive primes p_n and p_{n+1} such that
 *            k is neither squarefree nor a prime power
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;      /* array of primes up to current limit   */
static size_t    prime_cnt = 0;      /* number of primes in the array         */
static idx_t     prime_limit = 0;    /* highest number sieved                 */

/* Ensure that at least (n+1) primes are available. */
static void
ensure_primes(idx_t n)
{
    /* If we already have enough primes, nothing to do. */
    if (prime_cnt >= (size_t)(n + 1))
        return;

    /* Estimate an upper bound for the (n+1)th prime. For n >= 6,
       p_n <= n (log n + log log n).  Add a safety margin. */
    double nl = (double)n;
    double estimate = 15.0;
    if (n >= 6) {
        estimate = nl * (log(nl) + log(log(nl))) + 3.0 * nl;
    }
    idx_t new_limit = (idx_t)estimate;
    if (new_limit < 15) new_limit = 15;
    if (new_limit <= prime_limit)
        new_limit = prime_limit * 2 + 1;

    /* Rebuild prime list at the new limit. */
    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;
    build_primes_upto(new_limit, &new_primes, &new_cnt);

    /* Free old list and update globals. */
    if (primes) free(primes);
    primes     = new_primes;
    prime_cnt  = new_cnt;
    prime_limit = new_limit;
}

/* Return a(n) as defined in the OEIS entry. */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n<1; return 0 as safe fallback */

    /* Ensure we have enough primes to cover the n-th and (n+1)-th primes. */
    ensure_primes(n);

    /* If prime list still insufficient, grow it monotonically until it is. */
    while (prime_cnt < (size_t)(n + 1)) {
        idx_t new_limit = prime_limit * 2 + 1;
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(new_limit, &new_primes, &new_cnt);
        if (primes) free(primes);
        primes = new_primes;
        prime_cnt = new_cnt;
        prime_limit = new_limit;
    }

    uint64_t p_low  = primes[n - 1];
    uint64_t p_high = primes[n];
    seq_t count = 0;

    for (uint64_t m = p_low + 1; m < p_high; ++m) {
        /* Skip primes; they are squarefree and prime powers. */
        if (_smallest_prime_factor(m) == m)
            continue;

        uint64_t t       = m;
        bool     repeated = false;
        size_t   distinct  = 0;

        while (t > 1) {
            uint64_t p = _smallest_prime_factor(t);
            int exp   = 0;
            while (t % p == 0) {
                t /= p;
                ++exp;
            }
            if (exp > 1)
                repeated = true;
            ++distinct;
        }

        /* Count iff at least two distinct primes and at least one repetition. */
        if (distinct >= 2 && repeated)
            ++count;
    }

    return count;
}
