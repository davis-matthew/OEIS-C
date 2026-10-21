/*
 * A266475  Sum of the parts i_1 + i_2 + ... + i_{A001222(n)} of the unique
 *          strict partition with encoding n = Product_{j=1..A001222(n)}
 *          prime(i_j-j+1).
 * https://oeis.org/A266475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1618, u16 n <= 9894, u32 n <= 9894, u64 n <= 9894,
 *            u128 n <= 9894, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A266475
    a(n) = Sum of the parts i_1 + … + i_{Ω(n)} of the unique strict
    partition with encoding n = ∏_{j=1..Ω(n)} prime(i_j−j+1).
    Equivalently, if p_1,…,p_t are the prime factors of n counted with
    multiplicity (t = Ω(n)) and idx(p) is the index of prime p (2→1,3→2,…),
    then
        a(n) = Σ_{j=1}^t idx(p_j) + t·(t−1)/2 .
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility to build the list of primes up to a limit */
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;   /* array of primes (2,3,5,…) up to current limit   */
static size_t    primes_cnt = 0;  /* number of primes stored                       */
static idx_t     primes_limit = 0;/* largest prime value precomputed               */

/* Return the index (1‑based) of prime p, assuming primes array covers p */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt - 1, mid;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        uint64_t val = primes[mid];
        if (val == p) {
            return mid + 1;           /* 1‑based index */
        } else if (val < p) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return 0; /* should never happen for valid inputs */
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 */
        return 0;

    /* Ensure we have all primes up to n for indexing */
    if (primes == NULL || n > primes_limit) {
        free(primes);
        build_primes_upto(n, &primes, &primes_cnt);
        primes_limit = n;
    }

    idx_t temp = n;              /* remaining part to factor */
    idx_t omega = 0;             /* # of prime factors (with multiplicity) */
    seq_t sum_idx = 0;           /* sum of prime indices */

    /* Factor using the precomputed primes */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t pp = p * p;
        if (pp > (uint64_t)temp) break;

        while (temp % p == 0) {
            ++omega;
            sum_idx += (seq_t)(i + 1);  /* i is 0‑based; index = i+1 */
            temp /= p;
        }
    }

    /* Remaining factor > 1 is prime */
    if (temp > 1) {
        ++omega;
        sum_idx += (seq_t)(prime_index(temp));
    }

    /* a(n) = sum_idx + omega*(omega-1)/2 */
    seq_t omega_seq = (seq_t)omega;
    seq_t part2 = omega_seq * (omega_seq - 1u) / 2u;
    return sum_idx + part2;
}
