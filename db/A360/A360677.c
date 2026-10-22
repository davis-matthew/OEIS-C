/*
 * A360677  Sum of the right half (exclusive) of the prime indices of n.
 * https://oeis.org/A360677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the right half (exclusive) of the prime indices of
 *            n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the sum of the right half (exclusive) of the prime indices of n.
 * The prime index of a prime p is the number of primes ≤ p.
 *
 * Let the prime indices of n be the multiset
 *   { index(p) repeated exp(p) times } for each prime factor p of n.
 * Sort this multiset in non‑decreasing order.  Let its length be L.
 * Skip the first floor((L+1)/2) elements of the sorted multiset
 * and sum the remaining ones.  That sum is returned.
 *
 * For n = 1 the function returns 0 because there are no prime factors.
 *
 * The routine uses only the public utilities:
 *   factor_u64   – factorization of a 64‑bit integer
 *   prime_pi     – number of primes ≤ x (the prime index)
 *
 * Complexity is dominated by the factorization; for n up to 10 000
 * the routine is well under 2 ms per call.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Factorization arrays: the number of distinct factors of any 64‑bit value
       is at most 12, but we allocate a modest buffer of 64.              */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  count;

    factor_u64((uint64_t)n, primes, exps, &count);

    size_t len = 0;
    for (size_t i = 0; i < count; ++i) {
        len += (size_t)exps[i];
    }

    /* Number of elements to skip from the left half (exclusive).  */
    size_t skip = (len + 1) / 2;                /* floor((len+1)/2) */

    seq_t sum = (seq_t)0;

    for (size_t i = 0; i < count; ++i) {
        uint64_t p   = primes[i];
        int      e   = (int)exps[i];
        /* Index of prime p is the number of primes ≤ p.              */
        idx_t    idx = (idx_t)prime_pi(p);

        /* Skip some of the repeated idx values if needed.           */
        if (skip > 0) {
            size_t s = skip < (size_t)e ? skip : (size_t)e;
            skip -= s;
            e -= (int)s;
        }

        /* Add the remaining indices to the sum.                     */
        if (e > 0) {
            sum += (seq_t)idx * (seq_t)e;
        }
    }

    return sum;
}
