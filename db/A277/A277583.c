/*
 * A277583  Goldbach's problem extended to squares of prime gaps (>=2):
 *          smallest integer >= ((A078587(n) - A078496(n))^2)/n for n >= 4.
 * https://oeis.org/A277583
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(((A078496(n)-A078587(n))^2)/n) where A078587(n) <
 *            A078496(n) are the primes of the Goldbach partition of 2n that
 *            minimize |q‑p| (i.e. p is the largest prime less than n for
 *            which 2n‑p is prime)
 * Range:     u8 n <= 18, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t) */
#include "../utils/build_primes_upto.h"/* void build_primes_upto(idx_t, uint64_t **, size_t *) */

static idx_t   cached_limit = 0;   /* largest 2*n for which primes are built */
static uint64_t *cached_primes = NULL;
static size_t   cached_cnt       = 0;

/* Ensure we have a prime list up to 2*limit.  Returned array is sorted in
   ascending order.  O(number of primes up to 2*limit). */
static void ensure_limit(idx_t limit)
{
    idx_t need = limit + limit;   /* 2*limit, careful about wrap‑around */
    if (cached_limit < need) {
        /* free previous list if any */
        if (cached_primes)
            free(cached_primes);
        build_primes_upto(need, &cached_primes, &cached_cnt);
        cached_limit = need;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* sequence starts at n = 4 */

    /* Build primes up to 2*n at most once. */
    ensure_limit(n);

    /* Find the largest prime p < n such that q = 2n-p is also prime. */
    idx_t p = 0;
    for (size_t i = cached_cnt; i-- > 0;) {
        idx_t cand = cached_primes[i];
        if (cand >= n)
            continue;               /* we need p < n                */
        idx_t q = (idx_t)(n + n) - cand; /* q = 2n - cand            */
        if (is_prime(q)) {
            p = cand;
            break;
        }
    }

    if (p == 0)          /* should never happen for n >= 4 */
        return 0;

    idx_t q = (idx_t)(n + n) - p;
    seq_t diff = (seq_t)(q - p);              /* positive difference      */
    seq_t sq   = diff * diff;                 /* square of difference     */
    seq_t val  = (sq + (seq_t)n - 1) / (seq_t)n;  /* ceil(sq / n)           */
    return val;
}
