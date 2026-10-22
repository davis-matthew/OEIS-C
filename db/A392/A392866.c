/*
 * A392866  a(n) = A276086(n) mod A117366(A276086(n)), where A117366 gives the
 *          smallest prime greater than the largest prime dividing n, and
 *          A276086 is the primorial base exp-function.
 * https://oeis.org/A392866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9890, u16 n <= 9890, u32 n <= 9890, u64 n <= 9890,
 *            u128 n <= 9890, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A392866
 *  a(n) = A276086(n) mod A117366(A276086(n))
 *  where A276086(n) is the primorial‑base base‑product of n, and
 *  A117366(m) gives the smallest prime greater than the largest prime
 *  divisor of m.
 *
 *  Algorithm:  obtain the primorial‑base digits of n, noting the
 *  largest prime used.  The modulus is the next prime after that
 *  prime (or 2 if n = 0).  Compute the primorial‑base product
 *  modulo this modulus by multiplying each prime raised to its
 *  digit power, all reductions performed modulo the modulus.
 *  Complexity:  O(number of digits in the primorial expansion).
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* get_prime(k): kth prime, 1‑based */

/* Return the required term.  No external state is used. */
seq_t a(idx_t n)
{
    /* Special case n = 0:  A276086(0) = 1, largest prime divisor = none,
     * so A117366(1) = 2.  1 mod 2 = 1. */
    if (n == 0)
        return (seq_t)1;   /* 1 mod 2 == 1 */

    /* Find the largest prime index used in the primorial base
     * representation of n.  (i is 0‑based here, so get_prime(i+1)). */
    idx_t max_i = 0;
    idx_t t = n;
    idx_t i = 0;
    while (t > 0) {
        uint32_t prime = get_prime(i + 1);
        uint32_t digit = (uint32_t)(t % prime);
        if (digit != 0)
            max_i = i;      /* keep the last (hence largest) index */
        t /= prime;
        ++i;
    }

    /* Modulus is the next prime after the largest one used. */
    uint32_t mod = get_prime(max_i + 2);   /* max_i+1 is last used prime,
                                             so +1 gives next prime. */

    /* Compute the primorial‑base product modulo mod. */
    uint64_t result = 1ULL % mod;
    t = n;
    i = 0;
    while (t > 0) {
        uint32_t prime = get_prime(i + 1);
        uint32_t digit = (uint32_t)(t % prime);
        if (digit != 0) {
            uint64_t base = prime % mod;
            uint64_t powv = 1ULL % mod;
            uint32_t e = digit;
            while (e) {
                if (e & 1)
                    powv = (powv * base) % mod;
                e >>= 1;
                if (e)
                    base = (base * base) % mod;
            }
            result = (result * powv) % mod;
        }
        t /= prime;
        ++i;
    }

    return (seq_t)result;
}
