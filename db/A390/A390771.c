/*
 * A390771  a(n) is the minimal concatenation pq of two primes p <= q such
 *          that p + q = 2*n.
 * https://oeis.org/A390771
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal decimal concatenation of two primes p ≤ q * where
 *            p + q = 2*n
 * Range:     u8 n <= 6, u16 n <= 494, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/concat_parts.h"

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;            /* undefined for n < 2 (not used in tests) */
    }

    idx_t target = 2 * n;     /* even number to be split into primes */
    seq_t best = 0;
    bool found = false;

    /* search all possible prime splits p + q = target, p ≤ q */
    for (idx_t p = 2; p <= target / 2; ++p) {
        if (!is_prime(p))
            continue;          /* p must be prime */
        idx_t q = target - p;
        if (!is_prime(q))
            continue;          /* q must be prime */
        seq_t val = concat_parts(p, q);   /* decimal concatenation */
        if (!found || val < best) {
            best = val;
            found = true;
        }
    }

    return best;               /* guaranteed to be found for n ≥ 2 */
}
