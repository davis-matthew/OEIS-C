/*
 * A325036  Difference between product and sum of prime indices of n.
 * https://oeis.org/A325036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = product of prime indices of n – sum of prime indices of n
 *            (prime indices of n are a multiset consisting of the indices of
 *            the prime factors counted with multiplicity)
 * Range:     i8 n <= 1182, i16 n <= 9173, i32 n <= 9173, i64 n <= 9173,
 *            i128 n <= 9173, big any n
 * Helpers:   build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_uint.h"

static uint64_t *prime_list = NULL;   /* array of primes ≤ current limit */
static size_t      prime_cnt  = 0;     /* number of primes in prime_list    */
static idx_t       prime_limit = 0;    /* maximum prime value stored         */

/* Build a list of all primes up to \a limit and store it in the static
   variables.  Any previously allocated list is freed. */
static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_list)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Return the 1‑based index of the prime \a p.  \a p is guaranteed to be
   prime and ≤ prime_limit. */
static idx_t
prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (idx_t)(lo + 1);   /* indices are 1‑based */
}

seq_t
a(idx_t n)
{
    /* By definition a(1) = 1 (empty product minus empty sum). */
    if (n == 1)
        return (seq_t)1;

    /* For all n ≥ 2, we need all primes up to n for factorisation and
       index lookup. */
    ensure_primes_upto(n);

    idx_t   tmp   = n;
    seq_t   prod  = 1;
    seq_t   sum   = 0;

    /* Trial division by the primes we have. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > (uint64_t)tmp)   /* stop once p² > remaining factor */
            break;
        if (tmp % p != 0)
            continue;
        idx_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        idx_t idx = prime_index(p);
        prod *= pow_uint((uint64_t)idx, exp);
        sum  += (seq_t)idx * (seq_t)exp;
    }

    /* If anything remains it is a prime larger than sqrt(n). */
    if (tmp > 1) {
        uint64_t p = tmp;
        idx_t idx = prime_index(p);
        prod *= idx;            /* exp is 1, so pow_uint(idx,1)==idx */
        sum  += (seq_t)idx;
    }

    return prod - sum;
}
