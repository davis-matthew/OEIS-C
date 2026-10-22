/*
 * A325988  Number of covering (or complete) factorizations of n.
 * https://oeis.org/A325988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   cover_factorization, factor_u64, rec_cover
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of covering factorizations of n:
   a(n) = ∏_{p^e‖n} f(e)  where f(e) counts the unordered
   factorizations of p^e into factors >1 such that every divisor
   of n is a product of some submultiset of the factors.
   For a prime power p^e the exponent multiset must allow all
   sums 0..e by a subset of the exponents.  This is equivalent
   to a sorted multiset a1 ≤ a2 ≤ … with each ai ≤ (∑_{j<i} aj)+1.
   f(e) is computed by a tiny recursion based on that condition.   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/cover_factorization.h"
#include "../utils/rec_cover.h"

/* precompute f(e) for e up to 63 (exponents for n <= 10000 are < 14) */
static seq_t f_cache[64];
static bool f_init = false;
static void init_f_cache(void) {
    if (f_init) return;
    f_cache[0] = 1;                 /* unused */
    for (idx_t e = 1; e < 64; ++e) {
        f_cache[e] = cover_factorization(e);
    }
    f_init = true;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;          /* by definition */
    init_f_cache();

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t e = (idx_t)exps[i];
        result *= f_cache[e];
    }
    return result;
}
