/*
 * A257991  Number of odd parts in the partition having Heinz number n.
 * https://oeis.org/A257991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{p odd-indexed prime} v_p(n)
 * Range:     u8 n <= 9628, u16 n <= 9628, u32 n <= 9628, u64 n <= 9628,
 *            u128 n <= 9628, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where v_p(n) is the exponent of the prime p in the factorization of n.  */
/*  Compute a(n) by factoring n and summing the exponents of primes with odd
    index (prime_pi(p) odd).  */

#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t result = 0;
    idx_t m = n;

    /* Handle factor 2 separately (prime index 1 is odd) */
    idx_t e = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        ++e;
    }
    if (e > 0) result += (seq_t)e;   /* 2 is the 1st prime → odd index */

    /* Factor odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if ((prime_pi(p) & 1) != 0) result += (seq_t)exp;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        if ((prime_pi(m) & 1) != 0) result += 1;
    }

    return result;
}
