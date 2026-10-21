/*
 * A284761  a(n) = gcd(A279513(n), A279513(n+1)).
 * https://oeis.org/A284761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( product_{p|n} (p * exponent_of_p_in_n), *
 *            product_{q|(n+1)} (q * exponent_of_q_in_(n+1)) )
 * Range:     u8 n <= 5326, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, prime_tower_factorization
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/prime_tower_factorization.h"

seq_t a(idx_t n)
{
    seq_t v1 = prime_tower_factorization(n);
    seq_t v2 = prime_tower_factorization(n + 1ULL);
    return gcd(v1, v2);
}
