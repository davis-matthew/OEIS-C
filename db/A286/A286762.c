/*
 * A286762  Indices k such that A195441(k) = A195441(k+1).
 * https://oeis.org/A286762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the definition of OEIS sequence A286762.
   a(n) is the n-th integer k (offset 1) such that
   the set of primes p ≤ (k+2)/(2+(k mod 2)) with
   sum_of_digits(k+1 in base p) ≥ p
   is equal to the set of primes p ≤ (k+3)/(2+((k+1) mod 2))
   with sum_of_digits(k+2 in base p) ≥ p.  The sequence starts with
   a(1) = 0.  */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes   = NULL;    /* array of primes, sorted ascending */
static size_t    primes_cnt = 0;     /* number of primes stored */
static idx_t     primes_limit = 0;   /* maximal prime value stored */

/* Ensure that 'primes' contains all primes ≤ limit (inclusive). */
static void
ensure_primes(idx_t limit)
{
    if (limit <= primes_limit) return;

    /* Free old array and allocate new one. */
    if (primes) free(primes);
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* Compute sum of digits of 'n' in base 'base'. */
static seq_t
digit_sum(idx_t n, uint64_t base)
{
    seq_t sum = 0;
    seq_t t   = (seq_t)n;
    while (t != 0) {
        sum += t % base;
        t   /= base;
    }
    return sum;
}

/* Return true iff the prime sets for n and n+1 (k = n-1) are equal. */
static bool
prime_sets_equal(idx_t n)
{
    /* Bounds for the two consecutive integers. */
    idx_t denom1 = 2 + ((n - 1) & 1);
    idx_t bound1 = (n + 1) / denom1;

    idx_t denom2 = 2 + ((n) & 1);          /* n+1 has index n -> mod is n&1 */
    idx_t bound2 = (n + 2) / denom2;

    idx_t maxBound = (bound1 > bound2) ? bound1 : bound2;
    ensure_primes(maxBound);

    /* Compare each prime up to maxBound. */
    for (size_t i = 0; i < primes_cnt && primes[i] <= maxBound; ++i) {
        uint64_t p = primes[i];
        bool inc1 = (p <= bound1) && (digit_sum(n, p) >= (seq_t)p);
        bool inc2 = (p <= bound2) && (digit_sum(n + 1, p) >= (seq_t)p);
        if (inc1 != inc2) return false;
    }
    return true;
}

/* Return the n-th element of A286762 (1‑based index). */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but safe */

    idx_t target = n;    /* how many valid indices to skip */
    idx_t found  = 0;    /* number of indices found so far */
    idx_t cur_n  = 1;    /* current integer n = k+1 */

    while (true) {
        if (prime_sets_equal(cur_n)) {
            ++found;
            if (found == target) {
                /* k = n - 1 */
                return (seq_t)(cur_n - 1);
            }
        }
        ++cur_n;
    }
}
