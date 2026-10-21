/*
 * A265790  Numerators of lower primes-only best approximates (POBAs) to
 *          sqrt(8); see Comments.
 * https://oeis.org/A265790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 13, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of lower primes-only best approximates to sqrt(8)
   (OEIS A265790).  The algorithm generates the fractions
   p_k / q_k inductively:
   - p_1/q_1 = 5/2  (the smallest prime < sqrt(8)*2).
   - For k ≥ 1, let q_{k+1} be the least prime q > q_k such that
     ∃ a prime p with  p_k/q_k < p/q < sqrt(8).
   - Once such a q is found, choose p = the largest prime ≤ floor(√(8)·q).
   The numerator sequence is {p_k}.

   Initial values:
   a(1)=5, a(2)=13, a(3)=19, a(4)=31, a(5)=223, ...
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/next_prime.h"
#include "../utils/floor_sqrt.h"

/* Global cache of primes up to the current bound. */
static uint64_t *primes      = NULL;
static size_t   prime_cnt    = 0;
static uint64_t prime_limit  = 0;

/* Ensure that all primes ≤ 'limit' are available in the global array. */
static void
ensure_prime_limit(uint64_t limit)
{
    if (limit <= prime_limit) {
        return;
    }
    uint64_t *tmp;
    size_t   cnt;
    build_primes_upto(limit, &tmp, &cnt);
    free(primes);
    primes     = tmp;
    prime_cnt  = cnt;
    prime_limit= limit;
}

/* Return the index of the first prime > val in the prime array. */
static size_t
upper_bound_prime(uint64_t val)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* first index with prime > val, could be prime_cnt */
}

/* Core sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for index 0, return 0 as a sentinel. */
        return (seq_t)0;
    }

    /* The first fraction is 5/2. */
    seq_t pk = (seq_t)5;     /* numerator */
    uint64_t qk = 2;         /* denominator */

    if (n == 1) {
        return pk;
    }

    /* We will generate terms up to the required index. */
    for (idx_t k = 2; k <= n; ++k) {
        /* Find the next denominator prime. */
        uint64_t q = next_prime(qk);
        bool found = false;

        while (!found) {
            /* Compute upper = ⌊√(8 q²)⌋, which is the largest integer
               less than sqrt(8) * q. */
            seq_t sq   = (seq_t)8 * (seq_t)q * (seq_t)q;
            seq_t upper = floor_sqrt(sq);

            /* Ensure we have primes up to 'upper' (converted to uint64_t). */
            if ((seq_t)(UINT64_MAX) < upper) {
                /* For very large values this routine will not work.
                   In practice with n ≤ 10000 this condition never fires. */
                return (seq_t)0;
            }
            uint64_t upper64 = (uint64_t)upper;
            ensure_prime_limit(upper64);

            /* Find the largest prime ≤ upper. */
            size_t pos = upper_bound_prime(upper64);
            if (pos == 0) {
                /* No prime ≤ upper – advance to the next denominator prime. */
                q = next_prime(q);
                continue;
            }
            seq_t p_candidate = (seq_t)primes[pos - 1];

            /* Verify that the candidate actually yields a fraction > pk/qk. */
            /* Compare p_candidate / q with pk / qk  →  p_candidate * qk > pk * q */
            seq_t lhs = p_candidate * (seq_t)qk;
            seq_t rhs = pk * (seq_t)q;
            if (lhs > rhs) {
                pk = p_candidate;
                qk = q;
                found = true;
            } else {
                /* This denominator does not provide a better approximation. */
                q = next_prime(q);
            }
        }
    }

    return pk;
}
