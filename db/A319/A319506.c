/*
 * A319506  Number of numbers of the form 2*p or 3*p between consecutive
 *          triangular numbers T(n - 1) < {2,3}*p <= T(n) with p prime.
 * https://oeis.org/A319506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of integers of the form 2·p or 3·p that lie in (T(n‑1),
 *            T(n)], where p is prime, counted without multiplicity
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses the provided helper build_primes_upto() to generate a
 * table of primes up to a needed limit, and counts them with a binary
 * search.  The distinctness correction is necessary only for the
 * number 6 = 2·3 = 3·2. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"  /* declares build_primes_upto */

/* static state for the prime table */
static uint64_t *primes     = NULL;   /* array of primes in ascending order   */
static size_t   primes_cnt = 0;      /* number of primes in the array        */
static idx_t    primes_limit = 0;     /* maximum prime that has been generated*/

/* Ensure that primes up to 'limit' (inclusive) are available. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;                     /* already covered */

    uint64_t *new_primes = NULL;
    size_t   new_cnt    = 0;
    /* build_primes_upto returns primes as uint64_t** */
    build_primes_upto(limit, &new_primes, &new_cnt);

    free(primes);
    primes     = new_primes;
    primes_cnt = new_cnt;
    primes_limit = limit;
}

/* Count the number of primes <= val (inclusive). */
static size_t
count_primes_le(uint64_t val)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;        /* number of primes <= val */
}

seq_t
a(idx_t n)
{
    if (n == 0)        /* outside the defined offset, safe default */
        return (seq_t)0;

    /* triangular numbers: T(k) = k*(k+1)/2 */
    uint64_t t_prev = ((uint64_t)(n-1)) * (uint64_t)n >> 1;          /* T(n-1) */
    uint64_t t_cur  = ((uint64_t)n) * ((uint64_t)(n)+1) >> 1;        /* T(n)   */

    /* The largest prime we might need is the floor of t_cur/2   */
    idx_t max_needed = (idx_t)(t_cur / 2);
    /* t_cur/3 is always <= t_cur/2, so we don't need a larger limit. */

    ensure_primes(max_needed);

    /* Count primes in (T(n-1)/2 , T(n)/2] (for 2·p) */
    uint64_t limit2_hi = t_cur / 2;
    uint64_t limit2_lo = t_prev / 2;
    size_t   cnt2 = count_primes_le(limit2_hi) - count_primes_le(limit2_lo);

    /* Count primes in (T(n-1)/3 , T(n)/3] (for 3·p) */
    uint64_t limit3_hi = t_cur / 3;
    uint64_t limit3_lo = t_prev / 3;
    size_t   cnt3 = count_primes_le(limit3_hi) - count_primes_le(limit3_lo);

    /* Correct for the duplicate 6 = 2·3 = 3·2 if it lies in the interval. */
    seq_t total = (seq_t)cnt2 + (seq_t)cnt3;
    if (t_prev < 6 && t_cur >= 6)
        total -= (seq_t)1;

    return total;
}
